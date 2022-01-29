#pragma once
#include "Buttons.h"
#include "Arduino.h"

Buttons::Buttons () {};

void Buttons::init_io_buttons () {
  pinMode (CCW_BUTTON_PIN, INPUT_PULLUP);
  pinMode (CW_BUTTON_PIN, INPUT_PULLUP);
  pinMode (SWITCH_ENCODER_PIN, INPUT_PULLUP);
  pinMode (CCW_RELAY_PIN, OUTPUT);
  pinMode (CW_RELAY_PIN, OUTPUT);
  pinMode (CCW_ENCODER_PIN, INPUT_PULLUP);
  pinMode (CW_ENCODER_PIN, INPUT_PULLUP);
  pinMode (SWITCH_ENCODER_PIN, INPUT_PULLUP);

  digitalWrite (CCW_RELAY_PIN, HIGH);
  digitalWrite (CW_RELAY_PIN, HIGH);
}

bool Buttons::button_pressed (byte button) {
    bool pressed = digitalRead (button) == LOW;
  
  if (pressed) {
    delay (150);
    if (digitalRead (button) == LOW)
      return true;
  }
  return false;
}

void Buttons::debug_io_states () {
  if (button_pressed (CCW_BUTTON_PIN))
    Serial.println ("Pulsado CCW"); 
  if (button_pressed (CW_BUTTON_PIN))
    Serial.println ("Pulsado CW");
  if (button_pressed (SWITCH_ENCODER_PIN))
    Serial.println ("Pulsado SWITCH");   
}
