#ifndef I_SUBJECT_H
#define I_SUBJECT_H
#include <Arduino.h>
#include <IObserver.h>
template<typename S, typename E>

/**
 * @brief Interface for implementing the Observer pattern.
 */
class ISubject{ //Interface

public:
    virtual ~ISubject(){}; //Destructor
    virtual void subscribe(E event, IObserver<S,E>* observer) = 0;
    virtual void unsubscribe(E event, IObserver<S,E>* observer) = 0;
    virtual void notify(E event) = 0;
};

#endif