#ifndef I_SIREN_H
#define I_SIREN_H
#include <Arduino.h>

class ISiren{ //Interface

public:
    virtual ~ISiren(){}; //Destructor

   /**
    * @brief Initializes the Siren 
    */
    virtual void init() = 0;

    /**
     * @brief Allows you to check the status of the siren.
     * @return true if active
     * @return false if inactive
     */
    virtual bool isActive() = 0;

    /**
     * @brief Turns the siren on 
     */
    virtual void turnOn() = 0;

    /**
     * @brief Turns the siren off 
     */
    virtual void turnOff() = 0;
};

#endif