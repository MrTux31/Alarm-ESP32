#ifndef ALARM_INDICATOR_H
#define ALARM_INDICATOR_H

#include <Arduino.h>
#include <IObserver.h>
#include <AlarmManager.h>
#include <devices/Led.h>
#include <BaseLedIndicator.h>

/**
 * @class AlarmIndicator
 * @brief A concrete LED indicator representing the current alarm status via an LED.
 * Implements the IObserver interface, this class observes the alarm manager.
 */
class AlarmIndicator : public BaseLedIndicator, public IObserver<AlarmManager, AlarmManagerEvent> {
private:
    //LED blinking speed when alarm is arming
    static unsigned long constexpr ARMING_BLINKING_DELAY = 500;
    //LED blinking speed when an intrusion has been detected
    static unsigned long constexpr INTRUSION_BLINKING_DELAY = 100;

public:
    /**
     * @brief Constructor
     * @param pinLed The pin used for the led
     */
    AlarmIndicator(int pinLed);

    using BaseLedIndicator::update;

    /**
     * @brief Observer pattern method, called when an alarm manager event
     * is emitted.
     * It causes the indicator LED to react in different ways depending on the event.
     * For example, when the alarm is armed, the LED is simply on, 
     * the LED is off when disarmed and blinks when an intrusion is detected...
     */
    void update(AlarmManager *subject, AlarmManagerEvent event) override;

};

#endif