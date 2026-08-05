#include <AlarmManager.h>


AlarmManager::AlarmManager(ISiren& siren , std::vector<DetectionLoop> &loops, unsigned long sirenDurationMs) : _siren(siren),  _sirenDurationMs(sirenDurationMs)
, _loops(loops), _state(DISARMED) {

}

void AlarmManager::init(){
    //Initializing the siren
    _siren.init();

    //Initializing each loop
    for(DetectionLoop &loop : _loops){
        loop.init();
        //Subscribing to the events (for the observer pattern)
        loop.subscribe(TRIGGERED, this);
        loop.subscribe(PHYSICALLY_OPEN, this);
    }
}


void AlarmManager::update(){
    
    switch (_state){
        case DISARMED:
            //Waiting for armement 
            break;

        case ARMING:
            //The waiting period before arming has been exceeded
            if(millis() - _armingSince >= _armingDelayMs){
                _state = ARMED; //Alarm is now ARMED (and ready)
                //resetTriggers() ??
            } 
            break;
        
        case ARMED:
            updateAllLoops();
            //If a break is break is detected, this observer receives the event (in the second update method).
            break;
        
        case INTRUSION:
            if(!_isSirenTriggeredManually){ //Only process the automatic siren timeout if the user has not overridden it manually
                //The siren duration has been reached
                if(millis() - _intrusionStartedAt  >= _sirenDurationMs){
                    _siren.turnOff();
                    // Re-include any previously bypassed zone if it has been closed PHYSICALLY
                    updateAllLoops();

                    //We can rearm the alarm
                    if(canRearmAlarm()){_state = ARMED; }  //Alarm instantly rearmed
                    else{_state = STANDBY;} //No more loops available, we wait for loops to come back
                }
            }
            break;

        case STANDBY:
            //Updating to try to Re-include any previously bypassed zone if it has been closed PHYSICALLY
            updateAllLoops();
            //We can rearm the alarm
            if(canRearmAlarm()){
                _state = ARMED; //Alarm instantly rearmed
            }
            break;
    }
} 

void AlarmManager::update(DetectionLoop* loop, LoopEvent event){
    if(event == TRIGGERED){
        if (_state == ARMED) {
                //Disable the faulty loop to bypass it when we RE-ARM the alarm after 
                loop->disable();
                beginIntrusion();
                
        }
    }
    if(event == PHYSICALLY_OPEN){
        if (_state == INTRUSION){
                //Disable loops triggered DURING the intrusion (we don't want them to trigger the siren again)
                loop->disable();
        }
    }
    
}


void AlarmManager::armAlarm(){
    if(_state == DISARMED){
        _state = ARMING; //Start of the alarm arming procedure
        _armingSince = millis(); //Saving the start of arming
    }
    
}

void AlarmManager::updateAllLoops() {
    for(DetectionLoop &loop : _loops) {
        loop.update();
        loop.tryAutoReenable();
    }
}

bool AlarmManager::canRearmAlarm(){
    //Check if there are any non-bypassed loops remaining.
    for(DetectionLoop &loop : _loops){
        if(loop.isEnabled()){ //Minimum one has been found
            return true;
        }
    }
    return false;
}

void AlarmManager::disarmAlarm(){
    _state = DISARMED;
    _isSirenTriggeredManually = false;
    _siren.turnOff();
    for(DetectionLoop &loop : _loops){
        loop.resetTrigger(); //Reset the trigger  for each loop
        loop.enable(); //Reset the bypass 
    }     
}

void AlarmManager::resetAlarm(){
    for(DetectionLoop &loop : _loops){
        loop.resetTrigger(); //Reset the trigger 
    }
}

void AlarmManager::beginIntrusion(){
    // Freeze all currently open zones to prevent them from re-triggering the alarm after this cycle ends
    for(DetectionLoop &loop : _loops){
        if(loop.isPhysicalOpen()){
            loop.disable();
        }
    }
    _state = INTRUSION;
    //Beginning of the siren triggering
    _siren.turnOn();
    _intrusionStartedAt = millis(); //Saving the start of the triggering
}

void AlarmManager::triggerSirenManually(bool turnOn){
    if(turnOn && !_isSirenTriggeredManually){
        _siren.turnOn();
    }
    // Prevent manual override from silencing the siren during an active intrusion sequence
    else if(!turnOn && _state != INTRUSION){ 
        _siren.turnOff();
    }
    _isSirenTriggeredManually = turnOn; 
    
}

bool AlarmManager::isSirenTriggeredManually(){
    return _isSirenTriggeredManually;
}

bool AlarmManager::isSirenActive(){
    return _siren.isActive();
}

std::vector<DetectionLoop*> AlarmManager::getTriggeredLoops(){
    std::vector<DetectionLoop*> triggeredLoops = {};
    for(DetectionLoop &loop : _loops){
        if(loop.isTriggered()){
            triggeredLoops.push_back(&loop);
        }
    }
    return triggeredLoops;

}

void AlarmManager::setSirenDuration(unsigned long durationMs){
    _sirenDurationMs = durationMs;
}

void AlarmManager::setArmingDelay(unsigned long delayMs){
    _armingDelayMs = delayMs;
}

void AlarmManager::setEntryDelay(unsigned long delayMs){
    for(DetectionLoop &loop : _loops){
        loop.setDelay(delayMs);
    }
}

SystemState AlarmManager::getCurrentState(){
    return _state;
}


