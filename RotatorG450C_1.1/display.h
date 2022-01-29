#ifndef Display_h
#define Display_h
#include "Arduino.h"

const uint8_t RS = 5;
const uint8_t EN = 7;
const uint8_t D4 = 13;
const uint8_t D5 = 15;
const uint8_t D6 = 17;
const uint8_t D7 = 19;

const uint8_t MAX_COL = 16;
const uint8_t MAX_ROW = 2;

class Display {
  public:
    Display ();    
    void init ();
    void clear_screen ();
    void display_center (String text, uint8_t row);
    void display_azimuth (String az);
    void display_route (String route);
    void display_move_rotor (String activity);
    void display_encoder_position (String pos);
    void display_overlap (String ovl);
    void display_init_azimuth ();
  private:
    void clear_row (uint8_t rowClean);
};

#endif
