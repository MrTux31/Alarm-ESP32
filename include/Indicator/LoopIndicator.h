#ifndef LOOP_INDICATOR_H
#define LOOP_INDICATOR_H
#include <Arduino.h>
#include <Led.h>
#include <IObserver.h>
#include <DetectionLoop.h>
#include <AlarmManager.h>

class LoopIndicator : public IObserver<DetectionLoop, LoopEvent>, public IObserver<AlarmManager, AlarmManagerEvent>{

private:
    Led _led;
    DetectionLoop &_loop;

public:
    LoopIndicator(DetectionLoop &loop, int pinLed);

    void init();

    void update();

    //Update method for the observation of the loop
    void update(DetectionLoop *subject, LoopEvent event) override;
    
    //Update method for the observation of the manager
    void update(AlarmManager *subject, AlarmManagerEvent event) override;

};


#endif

