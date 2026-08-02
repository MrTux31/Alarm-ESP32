#include <Siren/RelaySiren.h>

RelaySiren::RelaySiren(int pin, bool activeLow){
    _pin = pin;
    _activeLow = activeLow;
    _isActive = false;
}

void RelaySiren::init(){
   pinMode(_pin, OUTPUT);
   turnOff();
}

bool RelaySiren::isActive(){
    return _isActive;
}

void RelaySiren::turnOn(){
    if(!_isActive){
        digitalWrite(_pin, _activeLow ? LOW : HIGH);
        _isActive = true;
    }
    
}

void RelaySiren::turnOff(){
    digitalWrite(_pin, _activeLow ? HIGH : LOW);
    _isActive = false;
    
    
}

