#include <Arduino.h>
#include <Siren/RelaySiren.h>
#include <AlarmManager.h>
#include <DetectionLoop.h>
#include <constants.h>
#include <vector>
#include <LoopIndicator.h>

//Functions declarations
void initIndicators();
void updateIndicators();

//Creating our siren
RelaySiren siren(PIN_RELAY_SIREN, false);

//Creating loops
DetectionLoop door(PIN_LOOP_1,"Main door",5000);
DetectionLoop window1(PIN_LOOP_2,"Window 1");
DetectionLoop window2(PIN_LOOP_3,"Window 2");
DetectionLoop window3(PIN_LOOP_4,"Window 3");

//Group all the loops together
std::vector<DetectionLoop*> loops = {&door,&window1,&window2,&window3};

//Creating the manager
AlarmManager manager(siren,loops);

AlarmManager::SystemState currentState;


//Indicators for each loops
LoopIndicator loopIndicator1(door,PIN_LED_B1);
LoopIndicator loopIndicator2(window1,PIN_LED_B2);
LoopIndicator loopIndicator3(window2,PIN_LED_B3);
LoopIndicator loopIndicator4(window3,PIN_LED_B4);
std::vector<LoopIndicator*> indicators = {&loopIndicator1,&loopIndicator2,&loopIndicator3,&loopIndicator4};


//Test button for manual trigger of the siren
bool lastButtonState = HIGH;


void setup() {
  Serial.begin(115200);

  //Initialization
  manager.init();
  initIndicators(); //Loop Indicators

  manager.setArmingDelay(5000);
  //Arming the alarm
  manager.armAlarm();

  //test button
  pinMode(27, INPUT_PULLUP);


}

void loop() {
  manager.update();
  updateIndicators(); //Loop Indicators
  currentState = manager.getCurrentState();

  //Test button
   bool reading = digitalRead(PIN_BTN_ARM);
   if (reading == LOW && lastButtonState == HIGH) {
     manager.triggerSirenManually(true);
   }else if (reading == HIGH && lastButtonState == LOW){
    manager.triggerSirenManually(false);
   }
   lastButtonState = reading;

  //Console tests

//   switch (currentState)
//   {
//     case  AlarmManager::DISARMED:
//       Serial.println("Disarmed");
//       break;
//     case  AlarmManager::ARMING:
//       Serial.println("Arming");
//       break;
//     case  AlarmManager::ARMED:
//       Serial.println("Armed");
//       break;
//     case AlarmManager::INTRUSION:
//       Serial.println("INTRUSION DETECTED !!!");
//       break;
//     default:
//       Serial.println("Standby");
//       break;
//     }

// }

}
void initIndicators(){
    for(LoopIndicator* &indicator : indicators){
        indicator->init();
        manager.subscribe(ALARM_ARMED, indicator);
        manager.subscribe(ALARM_DISARMED, indicator);
    }
  }

  void updateIndicators(){
    for(LoopIndicator* &indicator : indicators){
        indicator->update();
    }
  }

