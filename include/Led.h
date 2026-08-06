#ifndef LED_H
#define LED_H
#include <Arduino.h>


class Led{

public:
    enum Mode{ON, OFF, BLINKING};

    Led(int pin);

    void init();

    void update();

    void turnOn();
    
    void turnOff();

    bool isPhysicallyOn() const;

    void blink(unsigned long intervalMs) ;

    Mode getCurrentMode();

    int getPin();

private:
    int _pin;
    Mode _currentMode = OFF;
    unsigned long _blinkIntervalMs = 0;
    unsigned long _lastBlinkTime = 0;
};


#endif
