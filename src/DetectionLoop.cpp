#include <DetectionLoop.h>
#include <algorithm>

DetectionLoop::DetectionLoop(int pin, String name, unsigned long delayMs ,uint8_t mode) 
: _pin(pin), _name(name), _delayMs(delayMs), _mode(mode){
 
}

void DetectionLoop::init(){
    pinMode(_pin, _mode);
}

void DetectionLoop::update(){
    //The physical opening of the loop is detected
    if(isEnabled() && isPhysicalOpen() && !_wasOpened){
        _openedSince = millis(); //Used for delayed loops
        _wasOpened = true;
        // Notify the manager/observers that this loop has been opened (physically)
        notify(PHYSICALLY_OPEN);
        Serial.println("OUVERT");
    }

    if(isEnabled() && isTriggered() && !_wasTriggered){
        _wasTriggered = true;
        // Notify the manager/observers that this loop has been triggered
        notify(TRIGGERED);
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
    _wasTriggered = false;
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


//Overrided methods for the observer pattern/////////////
void DetectionLoop::subscribe(LoopEvent event, IObserver<DetectionLoop, LoopEvent>* observer){
    observers[event].push_back(observer);
}

void DetectionLoop::unsubscribe(LoopEvent event, IObserver<DetectionLoop, LoopEvent>* observer){
    //Getting the list of observers for the event
    auto& list = observers[event];
    //Finding et removing the right observer
    list.erase(
        std::remove(list.begin(), list.end(), observer),
        list.end()
    );
}

void DetectionLoop::notify(LoopEvent event){
    //Iterrating over the observers subscribed to the event
    for (auto* observer : observers[event]) {
        //Notify them that this loop was triggered
        observer->update(this,event);
    }
}


