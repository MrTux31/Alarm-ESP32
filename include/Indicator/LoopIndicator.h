#ifndef LOOP_INDICATOR_H
#define LOOP_INDICATOR_H
#include <Arduino.h>
#include <devices/Led.h>
#include <IObserver.h>
#include <devices/DetectionLoop.h>
#include <AlarmManager.h>
#include <BaseLedIndicator.h>

/**
 * @class LoopIndicator
 * @brief A concrete LED indicator representing the current status of a Detection loop via an LED.
 * Implements the IObserver interface, this class must observe only ONE DetectionLoop.
 */
class LoopIndicator : public BaseLedIndicator, public IObserver<DetectionLoop, LoopEvent>, public IObserver<AlarmManager, AlarmManagerEvent>{

private:
    //The instance of the alarm manager
    AlarmManager &_alarmManager;

public:
    /**
     * @brief Constructor
     * @param pinLed Pin used for the led
     * @param alarmManager The instance of the alarm manager 
     */
    LoopIndicator(int pinLed, AlarmManager &alarmManager);

    using BaseLedIndicator::update;
    
    /**
     * @brief Observer pattern method, called when an event
     * is emitted by the observed DetectionLoop.
     * 
     * It causes the indicator LED to react in different ways depending on the event.
     * For example, when the loop has been opened it makes the led blink, 
     * if the loop is triggered it makes the led turn on normally ...
     */
    void update(DetectionLoop *subject, LoopEvent event) override;
    
    /**
     * @brief Observer pattern method, called when an alarm manager event
     * is emitted. 
     * When the alarm has been disarmed, it turns off the led.
     */
    void update(AlarmManager *subject, AlarmManagerEvent event) override;

};


#endif

