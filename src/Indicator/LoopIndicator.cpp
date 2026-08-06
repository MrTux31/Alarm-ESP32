#include <LoopIndicator.h>

LoopIndicator::LoopIndicator(DetectionLoop &loop, int pinLed) : 
_led(pinLed), _loop(loop){

}

void LoopIndicator::init(){
    _led.init();
    _loop.subscribe(TRIGGERED, this);
    _loop.subscribe(PHYSICALLY_OPEN, this);

}

void LoopIndicator::update(){
    _led.update();
}

void LoopIndicator::update(DetectionLoop *subject, LoopEvent event){
    switch (event)
    {
    case PHYSICALLY_OPEN:
        Serial.println(String(_led.getPin()) + " clignotte");
        _led.blink(300);
        break;

    case TRIGGERED:
        Serial.println(String(_led.getPin()) + " allumée");
        _led.turnOn();
        break;

    }
}

void LoopIndicator::update(AlarmManager *subject, AlarmManagerEvent event){
    switch (event){
        case ALARM_DISARMED:
            _led.turnOff();
    }
}