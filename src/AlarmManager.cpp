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
            } 
            break;
        
        case ARMED:
            for(DetectionLoop &loop : _loops){
                //Updating each loop
                loop.update();
                //A break in the loop has been detected (only for the enabled ones)
                if(loop.isEnabled() && loop.isTriggered()){
                   loop.disable(); //Disable the faulty loop to bypass it when we RE-ARM the alarm after 
                   triggerSiren();
                   break;
                }
            }
            break;
        
        case SIREN_ACTIVE:
            //The siren duration has been reached
            if(millis() - _sirenActiveSince >= _sirenDurationMs){
                _siren.turnOff();
                bool canRearmAlarm = false;
                //Check if there are any non-bypassed loops remaining.
                for(DetectionLoop &loop : _loops){
                    if(loop.isEnabled()){ //Minimum one has been found
                        canRearmAlarm = true;
                        break;
                    }
                 }
                //We can rearm the alarm
                if(canRearmAlarm){
                    _state = ARMED; //Alarm instantly rearmed
                }else{ //No more loops available, we disarm
                    disarmAlarm();
                }
                
            }
            break;
    }
 
}


void AlarmManager::armAlarm(){
    if(_state == DISARMED){
        _state = ARMING; //Start of the alarm arming procedure
        _armingSince = millis(); //Saving the start of arming
    }
    
}

    
void AlarmManager::disarmAlarm(){
    _state = DISARMED;
    _siren.turnOff();
    resetAlarm();
}

void AlarmManager::resetAlarm(){
    for(DetectionLoop &loop : _loops){
        loop.resetTrigger(); //Reset the trigger 
        loop.enable(); //Reset the bypass 
    }
}

void AlarmManager::triggerSiren(){
    //Beginning of the siren triggering
    _state = SIREN_ACTIVE; 
    _siren.turnOn();
    _sirenActiveSince = millis(); //Saving the start of the triggering
}

bool AlarmManager::isSirenActive(){
    return _state == SIREN_ACTIVE;
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

State AlarmManager::getCurrentState(){
    return _state;
}


