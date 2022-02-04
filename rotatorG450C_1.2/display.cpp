#include <stdint.h>
#pragma once
#include "Display.h"
#include "Rotator.h"
#include "Arduino.h"
#include <LiquidCrystal.h>

const uint8_t RS = 5;
const uint8_t EN = 7;
const uint8_t D4 = 13;
const uint8_t D5 = 15;
const uint8_t D6 = 17;
const uint8_t D7 = 19;

uint8_t grados [8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B0000,
  B0000,
  B0000,
  B0000
};

LiquidCrystal lcd (RS, EN, D4, D5, D6, D7);

Display::Display () {};

void Display::init() {
  lcd.begin(MAX_COL, MAX_ROW);
  lcd.createChar (1, grados);
}

void Display::clear_row (uint8_t row) {
  for (uint8_t col = 0; col <= MAX_COL; col++) {
    lcd.setCursor (col, row);
    lcd.print (" ");
  }
}

void Display::clear_screen () {
  clear_row (UPPER_ROW);
  clear_row (LOWER_ROW);
}

void Display::display_center (String text, uint8_t row) {
  uint8_t colPosition = int ((MAX_COL - text.length())/2);
  
  clear_row (row);
  lcd.setCursor (colPosition, row);
  lcd.print (text); 
}

void Display::init_screen_azimuth () {
  clear_row (UPPER_ROW);
  lcd.setCursor (INIT_ROW, UPPER_ROW);
  lcd.print ("Dir =");
  lcd.setCursor (DEGREES_ROW, UPPER_ROW);
  lcd.write (1);
}

void Display::display_azimuth (String azimuth) {
  lcd.setCursor (COL_AZIMUTH, UPPER_ROW);
  lcd.print (azimuth);
}

void Display::display_overlap (String ovl) {
  lcd.setCursor (COL_OVERLAP, UPPER_ROW);
  lcd.print (ovl);
}

void Display::display_set_encoder () {
  lcd.setCursor (FIRST_COL, LOWER_ROW);
  lcd.print ("Ajustando a     ");
  lcd.setCursor (LAST_COL, LOWER_ROW);
  lcd.write (1); 
}

void Display::display_encoder_position (String pos) {
  lcd.setCursor (COL_ENCODER, LOWER_ROW);
  lcd.print (pos);    
}

void Display::display_init_turn () {
  lcd.setCursor (FIRST_COL, LOWER_ROW);
  lcd.print ("Girando a       ");
}

void Display::display_move_rotor (String dir) {
  lcd.setCursor (COL_TURN_TO, LOWER_ROW);
  lcd.print (dir); 
}
