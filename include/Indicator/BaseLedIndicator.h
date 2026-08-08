#ifndef BASE_LED_INDICATOR_H
#define BASE_LED_INDICATOR_H
#include <Arduino.h>
#include <devices/Led.h>

class BaseLedIndicator{

protected:
    Led _led;

public:
    virtual ~BaseLedIndicator() {}
    BaseLedIndicator(int pinLed);

    virtual void init();
    virtual void update();

};




#endif