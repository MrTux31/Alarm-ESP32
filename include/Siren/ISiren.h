#ifndef I_SIREN_H
#define I_SIREN_H
#include <Arduino.h>

class ISiren{ //Interface

public:
    virtual ~ISiren(){}; //Destructor

    virtual void init() = 0;

    virtual bool isRinging() = 0;

    virtual void turnOn() = 0;

    virtual void turnOff() = 0;
};

#endif