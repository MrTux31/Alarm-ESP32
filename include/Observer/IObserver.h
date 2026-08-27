#ifndef I_OBSERVER_H
#define I_OBSERVER_H
#include <Arduino.h>

/**
 * @brief Interface representing an observer in the Observer pattern.
 */
template<typename S, typename E>
class IObserver{ //Interface

public:
    virtual ~IObserver(){}; //Destructor
    /**
     * @brief Method to notify the current observer when an observed subject has emitted an
     * event.
     * @param subject The subject that emitted the event
     * @param event The emitted event
     */
    virtual void update(S *subject, E event) = 0;
};

#endif