#ifndef Rotator_h
#define Rotator_h
#include "Arduino.h"

typedef enum {
  CCW, CW
} rotation;

typedef enum {
  ANTENNA, ENCODER_SET, ENCODER_TO
} show;

typedef enum {
  MANUAL, PRESET
} move;

class Rotator {
  public:
    Rotator ();
    uint16_t read_antenna_position ();
    String int_to_right_string (uint16_t degreePosition);
    String int_to_left_string (uint16_t degreePosition);
    String degrees_to_route (uint16_t degreesPosition);   
  private:
};
#endif
