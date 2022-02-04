#ifndef Rotator_h
#define Rotator_h

#include "Arduino.h"

#define CCW_BUTTON_PIN 32
#define CW_BUTTON_PIN 34
#define CCW_RELAY_PIN 36
#define CW_RELAY_PIN 38
#define SWITCH_ENCODER_PIN 43
#define AZ_INPUT_PIN A0

typedef enum {
  CCW, CW
} rotation;

class Rotator {
  public:
    Rotator ();
    String int_to_right_string (uint16_t degreePosition);
    String int_to_left_string (uint16_t degreePosition);
    String degrees_to_route (uint16_t degreesPosition);
    void init_io_buttons ();
    uint16_t read_antenna_position ();
    bool button_pressed (byte button);
  private:
};

#endif
