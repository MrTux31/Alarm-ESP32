#include <Siren/RelaySiren.h>

RelaySiren::RelaySiren(int pin, bool activeLow){
    _pin = pin;
    _activeLow = activeLow;
    _isRinging = false;
}

void RelaySiren::init(){
   pinMode(_pin, OUTPUT);
   turnOff();
}

bool RelaySiren::isRinging(){
    return _isRinging;
}

void RelaySiren::turnOn(){
    if(!_isRinging){
        digitalWrite(_pin, _activeLow ? LOW : HIGH);
        _isRinging = true;
    }
    
}

void RelaySiren::turnOff(){
    digitalWrite(_pin, _activeLow ? HIGH : LOW);
    _isRinging = false;
    
    
}

