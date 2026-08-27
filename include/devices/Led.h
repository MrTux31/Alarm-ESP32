#ifndef LED_H
#define LED_H
#include <Arduino.h>

/**
 * @class Class representing a Led 
 */
class Led{

public:
    enum Mode{ON, OFF, BLINKING};

    /**
     * @brief Constructor
     * @param pin the pin used for the led
     */
    Led(int pin);

    /**
     * @brief Initializes the led
     */
    void init();

    /**
     * @brief Updates the led if in blinking mode,
     * checks if the blink delay has been reached.
     * Must be called in the main loop.
     */
    void update();

    /**
     * @brief Simply turns on the LED with a steady light
     */
    void turnOn();
    
    /**
     * @brief Turns the led off
     */
    void turnOff();

    /**
     * @brief Determines whether the LED is physically lit
     */
    bool isPhysicallyOn() const;

    /**
     * @brief Blinks the LED at a defined interval
 ​​    * 
     */
    void blink(unsigned long intervalMs) ;

    /**
     * @brief Returns the current operating mode of the LED
     * On, blinking, or off
     */
    Mode getCurrentMode();

    /**
     * @brief Returns the used pin for the led on the circuit
     */
    int getPin();

private:
    int _pin;
    Mode _currentMode = OFF;
    unsigned long _blinkIntervalMs = 0;
    unsigned long _lastBlinkTime = 0;
};


#endif
