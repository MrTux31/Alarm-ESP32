#include <Arduino.h>
#include <Siren/RelaySiren.h>
#include <AlarmManager.h>
#include <DetectionLoop.h>
#include <constants.h>
#include <vector>

//Creating our siren
RelaySiren siren(PIN_RELAY_SIREN, false);

//Creating loops
DetectionLoop door(PIN_LOOP_1,"Main door",20000);
DetectionLoop window1(PIN_LOOP_2,"Window 1");
DetectionLoop window2(PIN_LOOP_3,"Window 2");
DetectionLoop window3(PIN_LOOP_4,"Window 3");

//Group all the loops together
std::vector<DetectionLoop> loops = {door,window1,window2,window3};

//Creating the manager
AlarmManager manager(siren,loops);

SystemState currentState;

void setup() {
  Serial.begin(115200);

  //Initialization
  manager.init();
  manager.setArmingDelay(20000);
  //Arming the alarm
  manager.armAlarm();

}

void loop() {
  manager.update();
  currentState = manager.getCurrentState();

  //Console tests

  switch (currentState)
  {
    case  DISARMED:
      Serial.println("Disarmed");
      break;
    case  ARMING:
      Serial.println("Arming");
      break;
    case  ARMED:
      Serial.println("Armed");
      break;
    case INTRUSION:
      Serial.println("INTRUSION DETECTED !!!");
      break;
    default:
      break;
    }
}

