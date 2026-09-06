#ifndef CONSTANTS_H
#define CONSTANTS_H

// These items are examples! Adjust them to your own needs.

//Loops
const char* const NAME_LOOP_1 = "Entrance";
const char* const NAME_LOOP_2 = "Window living room";
const char* const NAME_LOOP_3 = "Window bedroom";
const char* const NAME_LOOP_4 = "Garage door";

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

//Time zone
const String POSIX_RULE = "CET-1CEST,M3.5.0,M10.5.0/3"; //Set your POSIX time zone

//Arm / Disarm Button
const int PIN_BTN_ARM  = 27; 


#endif
