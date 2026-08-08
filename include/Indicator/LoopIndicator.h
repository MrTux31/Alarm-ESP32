#ifndef LOOP_INDICATOR_H
#define LOOP_INDICATOR_H
#include <Arduino.h>
#include <devices/Led.h>
#include <IObserver.h>
#include <devices/DetectionLoop.h>
#include <AlarmManager.h>
#include <BaseLedIndicator.h>

class LoopIndicator : public BaseLedIndicator, public IObserver<DetectionLoop, LoopEvent>, public IObserver<AlarmManager, AlarmManagerEvent>{

private:
    AlarmManager &_alarmManager;

public:
    LoopIndicator(int pinLed, AlarmManager &alarmManager);

    using BaseLedIndicator::update;
    
    //Update method for the observation of the loop
    void update(DetectionLoop *subject, LoopEvent event) override;
    
    //Update method for the observation of the manager
    void update(AlarmManager *subject, AlarmManagerEvent event) override;

};


#endif

