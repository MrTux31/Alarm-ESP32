#include <LoopIndicator.h>

LoopIndicator::LoopIndicator(int pinLed, AlarmManager &alarmManager) : _alarmManager(alarmManager), 
BaseLedIndicator(pinLed){

}

void LoopIndicator::update(DetectionLoop *subject, LoopEvent event){
    switch (event)
    {
    case PHYSICALLY_OPEN:
        //If the alarm has not detected an intrusion yet we can start blinking
        if(_alarmManager.getCurrentState() != AlarmManager::INTRUSION){
            _led.blink(300);
        }else{
            _led.turnOn();
        }
        break;

    case TRIGGERED:
        //Switch to normal lighting when triggered
        _led.turnOn();
        break;

    }
}

void LoopIndicator::update(AlarmManager *subject, AlarmManagerEvent event){
    switch (event){
        case ALARM_DISARMED:
            _led.turnOff();
            break;
    
    }
}