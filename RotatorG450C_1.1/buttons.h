#ifndef Buttons_h
#define Buttons_h

#include "Arduino.h"

//#define DEBUG_IO_STATE
//#define DEBUG_ENCODER_POSITION

#define CCW_BUTTON_PIN 32
#define CW_BUTTON_PIN 34
#define CCW_RELAY_PIN 36
#define CW_RELAY_PIN 38
#define CCW_ENCODER_PIN 3
#define CW_ENCODER_PIN 2
#define SWITCH_ENCODER_PIN 43
#define AZ_INPUT_PIN A0

class Buttons {
  public:
    Buttons ();
    void init_io_buttons ();
    bool button_pressed (byte buton);
    void debug_io_states ();
  private:
};
#endif
