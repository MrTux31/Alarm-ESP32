#ifndef I_OBSERVER_H
#define I_OBSERVER_H
#include <Arduino.h>
template<typename S, typename E>
/**
 * @brief Interface for implementing the Observer pattern.
 */
class IObserver{ //Interface

public:
    virtual ~IObserver(){}; //Destructor
    virtual void update(S *subject, E event) = 0;
};

#endif