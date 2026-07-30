#ifndef ALARM_H
#define ALARM_H
#include <Arduino.h>

class Alarm{
private:

public:
    Alarm();

    bool isRinging();

    void turnOn();

    void turnOff();
};

#endif