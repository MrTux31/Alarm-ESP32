#include <IndicatorManager.h>

IndicatorManager::IndicatorManager(AlarmManager &alarmManager): _alarm(alarmManager){

}

void IndicatorManager::update(){
    for(LoopIndicator* indicator : _loopIndicators){
        indicator->update();
    }
    if(_alarmIndicator != nullptr){
        _alarmIndicator->update();
    }
}

void IndicatorManager::addLoopIndicator(LoopIndicator &loopIndicator, DetectionLoop &subjectLoop){
        loopIndicator.init();

        //Subscribing to the subject loop events
        subjectLoop.subscribe(TRIGGERED, &loopIndicator);
        subjectLoop.subscribe(PHYSICALLY_OPEN, &loopIndicator);
        //Subscribing to alarm events
        _alarm.subscribe(ALARM_DISARMED, &loopIndicator);
        _alarm.subscribe(ALARM_INTRUSION, &loopIndicator);

        //Adding the indicator to the list
        _loopIndicators.push_back(&loopIndicator);
    
   
}

void IndicatorManager::setAlarmIndicator(AlarmIndicator &alarmIndicator){
    _alarmIndicator = &alarmIndicator;
    _alarmIndicator->init();
    
    //Subscribing to alarm events
    _alarm.subscribe(ALARM_ARMED, &alarmIndicator);
    _alarm.subscribe(ALARM_DISARMED, &alarmIndicator);
    _alarm.subscribe(ALARM_ARMING, &alarmIndicator);
    _alarm.subscribe(ALARM_INTRUSION, &alarmIndicator);
    _alarm.subscribe(ALARM_STANDBY, &alarmIndicator);

   
}