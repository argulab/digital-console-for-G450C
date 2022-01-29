#pragma once
#include "Arduino.h"
#include "Rotator.h"
#include "Buttons.h"

Rotator::Rotator () {};

String Rotator::int_to_right_string (uint16_t degreePosition) {
  String formattedDegrees = String (degreePosition);
  
  if (degreePosition < 10) 
    return "  " + formattedDegrees;
    
  if (degreePosition >= 10 && degreePosition < 100)
    return " " + formattedDegrees;
    
  return formattedDegrees; 
}

String Rotator::int_to_left_string (uint16_t degreePosition) {
   String formattedDegrees = String (degreePosition);
  
  if (degreePosition < 10) 
    return formattedDegrees + "  ";
    
  if (degreePosition >= 10 && degreePosition < 100)
    return formattedDegrees + " ";
    
  return formattedDegrees;  
}

String Rotator::degrees_to_route (uint16_t degreesPosition) {
  if (degreesPosition <= 12 || degreesPosition >  349)  return "Norte";
  if (degreesPosition >  12 && degreesPosition <=  34)  return "NorNorEste";
  if (degreesPosition >  34 && degreesPosition <=  57)  return "Noreste";
  if (degreesPosition >  57 && degreesPosition <=  79)  return "EsteNorEste";
  if (degreesPosition >  79 && degreesPosition <= 102)  return "Este";
  if (degreesPosition > 102 && degreesPosition <= 124)  return "EsteSurEste";
  if (degreesPosition > 124 && degreesPosition <= 147)  return "SurEste";
  if (degreesPosition > 147 && degreesPosition <= 169)  return "SurSurEste";
  if (degreesPosition > 169 && degreesPosition <= 192)  return "Sur";
  if (degreesPosition > 192 && degreesPosition <= 214)  return "SurSurOeste";
  if (degreesPosition > 214 && degreesPosition <= 237)  return "SurOeste";
  if (degreesPosition > 237 && degreesPosition <= 259)  return "OesteSurEste";
  if (degreesPosition > 259 && degreesPosition <= 282)  return "Oeste";
  if (degreesPosition > 282 && degreesPosition <= 304)  return "OesteNorOeste";
  if (degreesPosition > 304 && degreesPosition <= 327)  return "NorOeste";
  if (degreesPosition > 327 && degreesPosition <= 349)  return "NorNorOeste";
}

uint16_t Rotator::read_antenna_position () {
  uint32_t rawSumma = 0;
  for (uint8_t i = 1; i <= 100; i++) {
    rawSumma += analogRead (AZ_INPUT_PIN);
  }
  
  float rawMedia = rawSumma / 100.0;
  float degreesRead = (rawMedia * 450.0) / 1023.0;

  return int( degreesRead);
}
