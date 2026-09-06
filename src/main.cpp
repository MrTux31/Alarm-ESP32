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
#include <service/BlynkService.h>
#include <CommunicationManager.h>
#include <WiFiManager.h>
#include <ezTime.h>
//Creating our siren
RelaySiren siren(PIN_RELAY_SIREN, false);

//Creating loops
DetectionLoop loop1(PIN_LOOP_1,NAME_LOOP_1,ENTRY_DELAY);
DetectionLoop loop2(PIN_LOOP_2,NAME_LOOP_2);
DetectionLoop loop3(PIN_LOOP_3,NAME_LOOP_3);
DetectionLoop loop4(PIN_LOOP_4,NAME_LOOP_4);

//Group all the loops together
std::vector<DetectionLoop*> loops = {&loop1,&loop2,&loop3,&loop4};

//Creating the alarm manager
AlarmManager alarmManager(siren,loops);
AlarmManager::SystemState currentState; //Only for console debug
AlarmManager::SystemState lastState = AlarmManager::DISARMED;//Only for console debug

//Creating led indicators
LoopIndicator loopIndicator1(PIN_LED_LOOP_1, alarmManager);
LoopIndicator loopIndicator2(PIN_LED_LOOP_2, alarmManager);
LoopIndicator loopIndicator3(PIN_LED_LOOP_3, alarmManager);
LoopIndicator loopIndicator4(PIN_LED_LOOP_4, alarmManager);
AlarmIndicator alarmIndicator(PIN_LED_ALARM_STATUS);

//Manager for the indicators
IndicatorManager indicatorManager(alarmManager);

//Setting up blynk
BlynkService blynk;
CommunicationManager::Config const blynkConfig = {
  .keys = {
    .status = "V0",
    .armDesarm = "V1",
    .manualMode = "V2",
    .triggeredLoop = "V3",
    .triggeredLoopNotification = "loop_triggered"
  },
  .values = {
    .arm = "1",
    .disarm = "0",
    .manualModeOn = "1",
    .manualModeOff = "0"
  }
};
CommunicationManager comManager(blynk, blynkConfig, alarmManager);

//State for the Arm / Disarm Button
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  //Wifi connection
  WiFiManager::getInstance()->init(WIFI_SSID, WIFI_PASS);

  //Blynk setup
  blynk.init(BLYNK_AUTH_TOKEN);
  comManager.init();
  comManager.setNotificationService(blynk); //To receive push notifications / mails 
  comManager.setPosix(POSIX_RULE); //Time zone

  //Setup led indicators
  indicatorManager.addLoopIndicator(loopIndicator1, loop1);
  indicatorManager.addLoopIndicator(loopIndicator2, loop2);
  indicatorManager.addLoopIndicator(loopIndicator3, loop3);
  indicatorManager.addLoopIndicator(loopIndicator4, loop4);
  indicatorManager.setAlarmIndicator(alarmIndicator);

  //Alarm setup
  alarmManager.init();
  alarmManager.setArmingDelay(ARMING_DELAY);

  //Button for arm / disarm
  pinMode(PIN_BTN_ARM, INPUT_PULLUP);

}

void loop() {
  alarmManager.update();
  indicatorManager.update(); //Update Indicators
  WiFiManager::getInstance()->update();
   if (WiFi.status() == WL_CONNECTED) {
      events(); //Updating ezTime            
  }
  comManager.update(); 

  currentState = alarmManager.getCurrentState();

  //Arm / Disarm Button
  bool reading = digitalRead(PIN_BTN_ARM);
  if (reading == LOW && lastButtonState == HIGH) {
    if(alarmManager.getCurrentState() == AlarmManager::DISARMED){
        alarmManager.armAlarm();
    }else{ alarmManager.disarmAlarm();}     
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

