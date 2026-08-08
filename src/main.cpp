#include <Arduino.h>
#include <devices/Siren/RelaySiren.h>
#include <AlarmManager.h>
#include <devices/DetectionLoop.h>
#include <constants.h>
#include <vector>
#include <LoopIndicator.h>
#include <BaseLedIndicator.h>
#include <AlarmIndicator.h>
#include <IndicatorManager.h>

//Creating our siren
RelaySiren siren(PIN_RELAY_SIREN, false);

//Creating loops
DetectionLoop door(PIN_LOOP_1,"Main door",20000);
DetectionLoop window1(PIN_LOOP_2,"Window 1");
DetectionLoop window2(PIN_LOOP_3,"Window 2");
DetectionLoop window3(PIN_LOOP_4,"Window 3");

//Group all the loops together
std::vector<DetectionLoop*> loops = {&door,&window1,&window2,&window3};

//Creating the alarm manager
AlarmManager alarmManager(siren,loops);
AlarmManager::SystemState currentState; //Only for console debug
AlarmManager::SystemState lastState = AlarmManager::DISARMED;//Only for console debug

//Creating led indicators
LoopIndicator loopIndicator1(PIN_LED_B1, alarmManager);
LoopIndicator loopIndicator2(PIN_LED_B2, alarmManager);
LoopIndicator loopIndicator3(PIN_LED_B3, alarmManager);
LoopIndicator loopIndicator4(PIN_LED_B4, alarmManager);
AlarmIndicator alarmIndicator(PIN_LED_ARME);

//Manager for the indicators
IndicatorManager indicatorManager(alarmManager);

//Test button for manual trigger of the siren
bool lastButtonState = HIGH;


void setup() {
  Serial.begin(115200);

  //Setup led indicators
  indicatorManager.addLoopIndicator(loopIndicator1, door);
  indicatorManager.addLoopIndicator(loopIndicator2, window1);
  indicatorManager.addLoopIndicator(loopIndicator3, window2);
  indicatorManager.addLoopIndicator(loopIndicator4, window3);
  indicatorManager.setAlarmIndicator(alarmIndicator);

  //Alarm setup
  alarmManager.init();
  alarmManager.setArmingDelay(10000);
  alarmManager.armAlarm();

  //test button
  pinMode(PIN_BTN_ARM, INPUT_PULLUP);


}

void loop() {
  alarmManager.update();
  indicatorManager.update(); //Update Indicators

  currentState = alarmManager.getCurrentState();

  //Test button
   bool reading = digitalRead(PIN_BTN_ARM);
   if (reading == LOW && lastButtonState == HIGH) {
     alarmManager.triggerSirenManually(true);
   }else if (reading == HIGH && lastButtonState == LOW){
    alarmManager.triggerSirenManually(false);
   }
   lastButtonState = reading;


  //Console tests
  if (currentState != lastState) {
    lastState = currentState;
    switch (currentState) {
        case AlarmManager::DISARMED:  Serial.println("Disarmed"); break;
        case AlarmManager::ARMING:    Serial.println("Arming"); break;
        case AlarmManager::ARMED:     Serial.println("Armed"); break;
        case AlarmManager::INTRUSION: Serial.println("INTRUSION DETECTED !!!"); break;
        default:                      Serial.println("Standby"); break;
    }
  }

}

