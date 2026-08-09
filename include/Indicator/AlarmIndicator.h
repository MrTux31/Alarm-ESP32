#ifndef ALARM_INDICATOR_H
#define ALARM_INDICATOR_H

#include <Arduino.h>
#include <IObserver.h>
#include <AlarmManager.h>
#include <devices/Led.h>
#include <BaseLedIndicator.h>

class AlarmIndicator : public BaseLedIndicator, public IObserver<AlarmManager, AlarmManagerEvent> {

public:
    AlarmIndicator(int pinLed);

    using BaseLedIndicator::update;

    //Update method for the observation of the manager
    void update(AlarmManager *subject, AlarmManagerEvent event) override;

};

#endif