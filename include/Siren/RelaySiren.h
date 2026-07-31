#ifndef RELAY_ALARM_H
#define RELAY_ALARM_H

#include <ISiren.h>
#include <Arduino.h>

/**
 * Implementation of the alarm interface;
 * represents a siren operating via a relay.
 */
class RelaySiren : public ISiren{ 

private:
    int _pin;
    bool _activeLow;
    bool _isRinging;

public:
    RelaySiren(int pin, bool activeLow = true); //Active low if the relay activates on a LOW state 

    void init() override;
    bool isRinging() override;
    void turnOn() override;
    void turnOff() override;

};


#endif