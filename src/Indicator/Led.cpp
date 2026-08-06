#include <Led.h>

Led::Led(int pin) : _pin(pin){
    
}

void Led::init(){
    pinMode(_pin, OUTPUT);
    turnOff();
}

void Led::update(){
    //Blinking mode
    if(_currentMode == BLINKING){
        //Checking the interval
        if(millis() - _lastBlinkTime >= _blinkIntervalMs){
            //Invert the state of the LED
            digitalWrite(_pin, isPhysicallyOn() ? LOW : HIGH);
            _lastBlinkTime = millis();
        }
    }
}

void Led::turnOn(){
    if(_currentMode != ON){
        _currentMode = ON;
        digitalWrite(_pin, HIGH);
    }
}

void Led::turnOff(){
    _currentMode = OFF;
    digitalWrite(_pin, LOW);
}

bool Led::isPhysicallyOn() const{
    return digitalRead(_pin) == HIGH;
}

void Led::blink(unsigned long intervalMs){
    _currentMode = BLINKING;
    _blinkIntervalMs = intervalMs;
    _lastBlinkTime = millis();
}

Led::Mode Led::getCurrentMode(){
    return _currentMode;
 }

int Led::getPin(){
    return _pin;
}