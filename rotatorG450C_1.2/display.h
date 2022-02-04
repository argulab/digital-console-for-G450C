#ifndef Display_h
#define Display_h

#include "Arduino.h"

#define MAX_ROW 2
#define MAX_COL 16
#define FIRST_COL 0
#define LAST_COL 15
#define UPPER_ROW 0
#define LOWER_ROW 1
#define INIT_ROW 0
#define DEGREES_ROW 9
#define COL_AZIMUTH 6
#define COL_OVERLAP 13
#define COL_ENCODER 12
#define COL_TURN_TO 10

class Display {
  public:
    Display ();
    void init();
    void clear_row (byte row);
    void clear_screen ();
    void display_center (String text, uint8_t row);
    void init_screen_azimuth ();
    void display_azimuth (String azimuth);
    void display_overlap (String ovl);
    void display_set_encoder ();
    void display_encoder_position (String pos);
    void display_init_turn ();
    void display_move_rotor (String dir);
  private:
};

#endif
