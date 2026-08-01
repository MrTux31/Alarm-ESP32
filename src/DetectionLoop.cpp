#include <DetectionLoop.h>


DetectionLoop::DetectionLoop(int pin, String name, unsigned long delayMs ,uint8_t mode) 
: _pin(pin), _name(name), _delayMs(delayMs), _mode(mode){
 
}

void DetectionLoop::init(){
    pinMode(_pin, _mode);
}

void DetectionLoop::update(){
    //The physical opening of the loop is detected
    if(isEnabled() && isPhysicalOpen() && !_wasOpened){
        _openedSince = millis();
        _wasOpened = true;
    }

    

}

void DetectionLoop::tryAutoReenable(){
    //Automatic reactivation of the loop if it has been closed and deactivated
    if(!isPhysicalOpen() && !isEnabled()){
        enable();
        resetTrigger();
    }
}

void DetectionLoop::disable(){
    _isEnabled = false;
}

void DetectionLoop::enable(){
    _isEnabled = true;
}

bool DetectionLoop::isEnabled() const{
    return _isEnabled;
}

void DetectionLoop::setDelay(unsigned long delayMs){
    _delayMs = delayMs;
}

bool DetectionLoop::isTriggered(){
    if(!_wasOpened){ //The loop has not been triggered
        return false;
    }
    return millis() - _openedSince >= _delayMs; //True if the delay has been exceeded
}

void DetectionLoop::resetTrigger(){
    _openedSince = 0;
    _wasOpened = false;
}

bool DetectionLoop::isPhysicalOpen(){
    return digitalRead(_pin) == HIGH;
}

String DetectionLoop::getName(){
    return _name;
}

void DetectionLoop::setName(String name){
    _name = name;
}


