#include "Rotator.h"
#include "Display.h"

const byte interrupt0Pin = 2;
const byte interrupt1Pin = 3;  

volatile uint16_t encoderPosition = 0; 
static bool rotating = false;

bool setCCW = false;
bool setCW = false;
bool settingPosition = false;

uint16_t oldEncoderPosition = 1;   
uint16_t oldAntennaPosition = 999;
String oldRoute = "";
bool overLap = false;

Rotator rtr = Rotator ();
Display dsp = Display ();

uint16_t read_antenna_position () {
  uint32_t rawSumma = 0;
  for (uint8_t i = 1; i <= 100; i++) {
    rawSumma += analogRead (AZ_INPUT_PIN);
  }
  
  float rawMedia = rawSumma / 100.0;
  uint16_t degreesRead = int ((rawMedia * 450.0) / 1023.0);

  overLap = degreesRead > 360;
  if (degreesRead > 360) degreesRead -= 360;
  return degreesRead;
}

void service_azimuth (uint16_t pos) {
  String formattedOverLap = overLap ? "OVL" : "   ";

  dsp.display_azimuth (rtr.int_to_right_string (pos));
  dsp.display_overlap (formattedOverLap);  
}

void on_change_antenna_position (uint32_t newPosition) {
  oldAntennaPosition = newPosition;
  service_azimuth (newPosition);
  String route = rtr.degrees_to_route (newPosition);
  if (route != oldRoute) {
    oldRoute = route;
    dsp.display_center (route, LOWER_ROW);    
  }  
}

void on_ccw_button_pressed () {
  if (rtr.button_pressed (CW_BUTTON_PIN))
    return;

  dsp.display_center ("Girando a CW", LOWER_ROW);
  digitalWrite (CCW_RELAY_PIN, LOW);    
 
  while (true) {
    if (rtr.button_pressed (CW_BUTTON_PIN)) 
      break;
    if (!rtr.button_pressed (CCW_BUTTON_PIN))
      break; 
    service_azimuth (read_antenna_position ());
  };
  
  digitalWrite (CCW_RELAY_PIN, HIGH);
  dsp.display_center (rtr.degrees_to_route (read_antenna_position ()), LOWER_ROW);
}

void on_cw_button_pressed () {
  if (rtr.button_pressed (CCW_BUTTON_PIN))
    return;

  dsp.display_center ("Girando a CCW", LOWER_ROW);
  digitalWrite (CW_RELAY_PIN, LOW);    
 
  while (true) {
    if (rtr.button_pressed (CCW_BUTTON_PIN)) 
      break;
    if (!rtr.button_pressed (CW_BUTTON_PIN))
      break;
 
    service_azimuth (read_antenna_position ());
  }; 
  digitalWrite (CW_RELAY_PIN, HIGH);  
  dsp.display_center (rtr.degrees_to_route (read_antenna_position ()), LOWER_ROW);
}

void do_encoder_ccw () {
  if (rotating) delay (1);  
  if (digitalRead (interrupt1Pin) != setCCW) { 
    setCCW = !setCCW;
    if (setCCW && !setCW) encoderPosition += 1;
    if (encoderPosition > 360) encoderPosition = 0;
    rotating = false; 
  }  
}

void do_encoder_cw () {
  if (rotating) delay (1);   
  if (digitalRead (interrupt0Pin) != setCW) { 
    setCW = !setCW;
    if (setCW && !setCCW) encoderPosition -= 1;
    if (encoderPosition > 360) encoderPosition = 360;
    rotating = false; 
  }  
}

void on_change_encoder_position () {
  if (encoderPosition == 0)
    return;

  if (settingPosition == false) {
    settingPosition = true;
    dsp.display_set_encoder();
  }
    
  dsp.display_encoder_position (rtr.int_to_right_string (encoderPosition));    
}

rotation check_move_to (uint16_t pos) {
  if (overLap) {
    if (encoderPosition > 90) return CCW;
    return encoderPosition > pos ? CW : CCW;
  }
  
  if (encoderPosition < 90) {
    return pos - encoderPosition < 180 ? CW : CCW;  
  }
  
  return encoderPosition > pos ? CW : CCW;
}

void on_switch_encoder_button_pressed () {
  uint16_t antennaPosition = read_antenna_position ();
  rotation moveTo = check_move_to (antennaPosition);
  uint8_t relayPin = moveTo == CW ? CW_RELAY_PIN : CCW_RELAY_PIN;
  digitalWrite (relayPin, LOW);
  
  dsp.display_init_turn ();
  dsp.display_move_rotor (rtr.int_to_left_string (encoderPosition));
  
  while (true) {     
    uint16_t antennaPosition = read_antenna_position ();
    
    if (rtr.button_pressed (SWITCH_ENCODER_PIN))
      break;
    if (antennaPosition == encoderPosition) 
      break;
      
    if (antennaPosition != oldAntennaPosition) {      
      oldAntennaPosition = antennaPosition;
      service_azimuth (antennaPosition);
    }
  }

  digitalWrite (relayPin, HIGH);
  rotating = false;
  settingPosition = false;
  encoderPosition = 0;
  dsp.display_center (rtr.degrees_to_route (antennaPosition), LOWER_ROW);
}

void setup() {
  uint32_t serialRate = 115200;
  Serial.begin (serialRate);
  Serial.flush();

  pinMode (interrupt0Pin, INPUT_PULLUP);
  pinMode (interrupt1Pin, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(interrupt0Pin), do_encoder_cw, CHANGE);
  attachInterrupt(digitalPinToInterrupt (interrupt1Pin), do_encoder_ccw, CHANGE);

  rtr.init_io_buttons ();
  dsp.init ();
  dsp.display_center ("YAESU G450C v1", 0);
  dsp.display_center ("by EA2J, 2022", 1);
  delay (2000);

  dsp.clear_screen ();
  dsp.init_screen_azimuth ();

  uint16_t antennaPosition = read_antenna_position ();
  on_change_antenna_position (antennaPosition);
}

void loop() {
  rotating = true;
  uint16_t antennaPosition = read_antenna_position ();

  if (encoderPosition != oldEncoderPosition)
    on_change_encoder_position ();

  if (rtr.button_pressed (SWITCH_ENCODER_PIN))
    on_switch_encoder_button_pressed ();

  if (antennaPosition != oldAntennaPosition) 
    on_change_antenna_position (antennaPosition);

  if (rtr.button_pressed (CCW_BUTTON_PIN))
    on_ccw_button_pressed ();

  if (rtr.button_pressed (CW_BUTTON_PIN))
    on_cw_button_pressed ();
}
