#ifndef RELAY_ALARM_H
#define RELAY_ALARM_H

#include <ISiren.h>
#include <Arduino.h>

/**
 * @class RelaySiren
 * @brief Implementation of the alarm interface
 * represents a siren operating via a relay.
 */
class RelaySiren : public ISiren{ 

private:
    int _pin;
    bool _activeLow;
    bool _isActive;

public:
    /**
    * @brief Constructor
    * @param pin The pin used for the siren
    * @param activeLow true if the relay activates on a LOW state 
    */
    RelaySiren(int pin, bool activeLow = true); 

    void init() override;
    bool isActive() override;
    void turnOn() override;
    void turnOff() override;

};


#endif