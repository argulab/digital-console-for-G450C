#pragma once
#include "Display.h"
#include <LiquidCrystal.h>
#include "Arduino.h"

LiquidCrystal lcd (RS, EN, D4, D5, D6, D7);

Display::Display () {}

byte grados [8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B0000,
  B0000,
  B0000,
  B0000
};

void Display::init() {
  lcd.begin(MAX_COL, MAX_ROW);
  lcd.createChar (1, grados);
}

void Display::clear_row (uint8_t rowClean) {
  for (uint8_t col = 0; col < MAX_COL; col++) {
    lcd.setCursor (col, rowClean);
    lcd.print (" ");
  }
}

void Display::clear_screen () {
  clear_row (0);
  clear_row (1);
}

void Display::display_center (String text, uint8_t row) {
  uint8_t colPosition = int ((MAX_COL - text.length())/2);
  
  clear_row (row);
  lcd.setCursor (colPosition, row);
  lcd.print (text); 
}

void Display::display_init_azimuth () {
  clear_row (0);
  lcd.setCursor (0, 0);
  lcd.print ("Dir=");
  lcd.setCursor (7, 0);
  lcd.write (1);
}

void Display::display_azimuth (String az) {
  lcd.setCursor (4, 0);
  lcd.print (az);
}

void Display::display_route (String route) {
  clear_row (1);
  lcd.setCursor (6 , 0);
  lcd.print (route);
}

void Display::display_overlap (String ovl) {
  lcd.setCursor (13, 0);
  lcd.print (ovl);
}

void Display::display_move_rotor (String direction) {
  clear_row (1);
  lcd.setCursor (0, 1);
  lcd.print ("Girando a " + direction); 
}

void Display::display_encoder_position (String pos) {
  clear_row (1);
  lcd.setCursor (0, 1);
  lcd.print ("Dir.hacia = ");
  lcd.print (pos);
}
