#ifndef BASE_LED_INDICATOR_H
#define BASE_LED_INDICATOR_H
#include <Arduino.h>
#include <devices/Led.h>

/**
 * @class BaseLedIndicator
 * Abstract class representing an indicator that uses an LED
 */
class BaseLedIndicator{

protected:
    Led _led;

public:
    virtual ~BaseLedIndicator() {}
    /**
     * @brief Constructor
     * @param pinLed The pin used for the led
     */
    BaseLedIndicator(int pinLed);

    /**
     * @brief Initializes the indicator 
     */
    virtual void init();

    /**
     * @brief Updates the indicator.
     * Must be called in the main loop
     */
    virtual void update();

};




#endif