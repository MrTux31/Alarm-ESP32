#include <LoopIndicator.h>

LoopIndicator::LoopIndicator(int pinLed, AlarmManager &alarmManager) : _alarmManager(alarmManager), 
BaseLedIndicator(pinLed){

}

void LoopIndicator::update(DetectionLoop *subject, LoopEvent event){
    switch (event)
    {
    case PHYSICALLY_OPEN:
        //If the alarm has not detected an intrusion yet
        if(_alarmManager.getCurrentState() != AlarmManager::INTRUSION){
            _led.blink(300);
        }else{
            _led.turnOn();
        }
        break;

    case TRIGGERED:
        _led.turnOn();
        break;

    
    //TODO : Gestion d'un event qui symbolise la fin du délai d'activation d'une loop
    //Glitch visuel loop avec délai : continue à clignotter meme après fin d'alarme
    

    }
}

void LoopIndicator::update(AlarmManager *subject, AlarmManagerEvent event){
    switch (event){
        case ALARM_DISARMED:
            _led.turnOff();
            break;
    }
}