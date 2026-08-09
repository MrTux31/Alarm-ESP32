#ifndef LED_H
#define LED_H
#include <Arduino.h>


class Led{

public:
    enum Mode{ON, OFF, BLINKING};

    Led(int pin);

    void init();

    void update();

    /**
     * @brief Simply turns on the LED with a steady light
     */
    void turnOn();
    
    void turnOff();

    /**
     * Determines whether the LED is physically lit
     */
    bool isPhysicallyOn() const;

    /**
     * @brief Blinks the LED at a defined interval
 ​​    * 
     */
    void blink(unsigned long intervalMs) ;

    /**
     * Returns the current operating mode of the LED
     * On, blinking, or off
     */
    Mode getCurrentMode();

    /**
     * Returns the used pin for the led on the circuit
     */
    int getPin();

private:
    int _pin;
    Mode _currentMode = OFF;
    unsigned long _blinkIntervalMs = 0;
    unsigned long _lastBlinkTime = 0;
};


#endif
