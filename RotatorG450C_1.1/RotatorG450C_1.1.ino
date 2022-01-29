/*29/01/2022 Primera compilación completa funcionando con el encoder 
Consola para el rotor Yaesu G450C por Arsenio Gutierrez, EA2J
*/

#include "Rotator.h"
#include "Buttons.h"
#include "Display.h"

bool setCCW = false;
bool setCW = false;
bool overLap = false;

volatile uint16_t encoderPosition = 0;  
uint16_t oldEncoderPosition = 1;   
static bool rotating = false;
show showControl = ANTENNA;

Rotator rtr = Rotator ();
Buttons but = Buttons ();
Display dsp = Display ();

uint16_t oldAntennaPosition;

void do_encoder_ccw () {
  if (rotating) delay (1);  
  if (digitalRead (CCW_ENCODER_PIN) != setCCW) { 
    setCCW = !setCCW;
    if (setCCW && !setCW) encoderPosition += 1;
    if (encoderPosition > 360) encoderPosition = 0;
    rotating = false; 
  }  
}

void do_encoder_cw () {
  if (rotating) delay (1);   
  if (digitalRead (CW_ENCODER_PIN) != setCW) { 
    setCW = !setCW;
    if (setCW && !setCCW) encoderPosition -= 1;
    if (encoderPosition > 360) encoderPosition = 360;
    rotating = false; 
  }  
}

void service_azimuth (uint16_t pos, move howMove) {      
  String formattedPosition = rtr.int_to_right_string (pos);
  String route = rtr.degrees_to_route (pos);
  String formattedOverLap = overLap ? "OVL" : "   ";

  dsp.display_azimuth (formattedPosition);
  dsp.display_overlap (formattedOverLap);

  if (howMove == MANUAL)
    dsp.display_center (rtr.degrees_to_route (pos), 1);
}

void on_ccw_button_pressed () {
  if (but.button_pressed (CW_BUTTON_PIN))
    return;

  dsp.display_move_rotor ("CCW");
  digitalWrite (CCW_RELAY_PIN, LOW);    
 
  while (true) {
    if (but.button_pressed (CW_BUTTON_PIN)) 
      break;
    if (!but.button_pressed (CCW_BUTTON_PIN))
      break;
  };
  
  dsp.display_center ("En marcha CCW", 1);   
  digitalWrite (CCW_RELAY_PIN, HIGH);
}

void on_cw_button_pressed () {
  if (but.button_pressed (CCW_BUTTON_PIN))
    return;

  dsp.display_move_rotor ("CW");
  digitalWrite (CW_RELAY_PIN, LOW);    
 
  while (true) {
    if (but.button_pressed (CCW_BUTTON_PIN)) 
      break;
    if (!but.button_pressed (CW_BUTTON_PIN))
      break;
  };
  
  dsp.display_center ("En marcha CW", 1);   
  digitalWrite (CW_RELAY_PIN, HIGH);
}

rotation check_move_to (uint16_t antenna) {
  if (overLap) {
    if (encoderPosition > 90) return CCW;
    return encoderPosition > antenna ? CW : CCW;
  }
  
  if (encoderPosition < 90) {
    return antenna - encoderPosition < 180 ? CW : CCW;  
  }
  
  return encoderPosition > antenna ? CW : CCW;
}

void on_button_encoder_pressed (rotation to) {
  String target = to == CW ? " CW" : " CCW";
  dsp.display_center ("Girando a " + String (encoderPosition) + target, 1);
  uint8_t relayPin = to == CW ? CW_RELAY_PIN : CCW_RELAY_PIN;

  while (true) {
    uint16_t antenna = rtr.read_antenna_position ();
    overLap = antenna > 360 ? true : false;

    if (antenna > 360) antenna -= 360;    
    service_azimuth (antenna, PRESET);
     
    if (but.button_pressed (SWITCH_ENCODER_PIN)) break;
    if (antenna == encoderPosition) break;
    digitalWrite (relayPin, LOW);
  }

  digitalWrite (relayPin, HIGH);
  uint16_t antennaPosition = rtr.read_antenna_position ();
  oldAntennaPosition = antennaPosition;
  service_azimuth (antennaPosition, MANUAL); 
}
  
void setup() {
  uint32_t serialRate = 115200;
  Serial.begin (serialRate);

  attachInterrupt(CCW, do_encoder_ccw, CHANGE);
  attachInterrupt(CW, do_encoder_cw, CHANGE);

  dsp.init ();
  dsp.display_center ("YAESU G450C v1", 0);
  dsp.display_center ("by EA2J, 2022", 1);
  delay (2000);
  dsp.clear_screen ();
  dsp.display_init_azimuth ();
  but.init_io_buttons ();

  uint16_t antennaPosition = rtr.read_antenna_position ();
  oldAntennaPosition = antennaPosition;
  service_azimuth (antennaPosition, MANUAL);
}

void loop() {
  #ifdef DEBUG_IO_STATE
    but.debug_io_states ();
  #endif  //DEBUG_IO_STATE

  rotating = true;
  uint16_t antennaPosition = rtr.read_antenna_position ();

  overLap = antennaPosition > 360;
  if (antennaPosition > 360) antennaPosition -= 360;

  if (antennaPosition != oldAntennaPosition) {
    oldAntennaPosition = antennaPosition;
    service_azimuth (antennaPosition, MANUAL);
  }

  if (encoderPosition != oldEncoderPosition) {

    #ifdef DEBUG_ENCODER_POSITION
      Serial.print("Encoder Position = ");
      Serial.println (encoderPosition);
      Serial.print ("Old encoder position = ");
      Serial.println (oldEncoderPosition);
    #endif  //DEBUG_ENCODER_POSITION

    oldEncoderPosition = encoderPosition;
    if (oldEncoderPosition != 0) {
      String formattedPosition = rtr.int_to_left_string (encoderPosition);
      dsp.display_encoder_position (formattedPosition);    
    }
  }

  if (but.button_pressed (SWITCH_ENCODER_PIN) && antennaPosition != encoderPosition) {
    rotation moveTo = check_move_to (antennaPosition); 
    on_button_encoder_pressed (moveTo);
  }

  if (but.button_pressed (CCW_BUTTON_PIN))
    on_ccw_button_pressed ();

  if (but.button_pressed (CW_BUTTON_PIN))
    on_cw_button_pressed ();
}
