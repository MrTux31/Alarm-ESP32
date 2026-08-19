#ifndef ALARM_INDICATOR_H
#define ALARM_INDICATOR_H

#include <Arduino.h>
#include <IObserver.h>
#include <AlarmManager.h>
#include <devices/Led.h>
#include <BaseLedIndicator.h>

class AlarmIndicator : public BaseLedIndicator, public IObserver<AlarmManager, AlarmManagerEvent> {
private:
    static unsigned long constexpr ARMING_BLINKING_DELAY = 500;
    static unsigned long constexpr INTRUSION_BLINKING_DELAY = 100;

public:
    AlarmIndicator(int pinLed);

    using BaseLedIndicator::update;

    //Update method for the observation of the manager
    void update(AlarmManager *subject, AlarmManagerEvent event) override;

};

#endif