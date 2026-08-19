#ifndef CONSTANTS_H
#define CONSTANTS_H

//Loops
const char* const NAME_LOOP_1 = "Entrée";
const char* const NAME_LOOP_2 = "Rez-de-chaussé";
const char* const NAME_LOOP_3 = "Étage";
const char* const NAME_LOOP_4 = "Compteur électrique";

const int PIN_LOOP_1 = 13; 
const int PIN_LOOP_2 = 12;
const int PIN_LOOP_3 = 14; 
const int PIN_LOOP_4 = 4;

const int PIN_LED_LOOP_1 = 25; 
const int PIN_LED_LOOP_2 = 33;
const int PIN_LED_LOOP_3 = 32; 
const int PIN_LED_LOOP_4 = 16;

//Alarm settings
const int PIN_LED_ALARM_STATUS = 26;
const int PIN_RELAY_SIREN = 18;
const unsigned long ARMING_DELAY = 25000; //Exit delay
const unsigned long ENTRY_DELAY = 12000; //Delay for the main door (to have the time to disarm)
const unsigned long DEFAULT_SIREN_DURATION_MS = 200000; 


const int PIN_BTN_ARM  = 27; 




#endif