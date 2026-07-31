#include <Arduino.h>
#include <Siren/RelaySiren.h>
#include <AlarmManager.h>
#include <DetectionLoop.h>
#include <constants.h>
#include <vector>


RelaySiren siren(PIN_RELAY_SIREN);
//Creating loops
DetectionLoop porte(PIN_LOOP_1,"Porte entree",20000);
DetectionLoop fenetre1(PIN_LOOP_2,"Fenetre 1");
DetectionLoop fenetre2(PIN_LOOP_3,"Fenetre 2");
DetectionLoop fenetre3(PIN_LOOP_4,"Fenetre 3");
std::vector<DetectionLoop> loops = {porte,fenetre1,fenetre2,fenetre3};

//Creating the manager
AlarmManager manager(siren,loops);

State currentState;

void setup() {
  Serial.begin(115200);

  //Initialization
  manager.init();
  manager.setArmingDelay(10000);
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
  case  SIREN_ACTIVE:
    Serial.println("SIREN ACTIVE !!!");
    break;
  default:
    break;
  }

}

