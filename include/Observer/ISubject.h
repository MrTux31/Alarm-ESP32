#ifndef I_SUBJECT_H
#define I_SUBJECT_H
#include <Arduino.h>
#include <IObserver.h>

/**
 * @brief Interface representing a subject in the Observer pattern.
 */
template<typename S, typename E>
class ISubject{ //Interface

public:
    virtual ~ISubject(){}; //Destructor
    /**
     * @brief Subscribes an observer to the current subject on a 
     * specific event.
     * @param event The event to listen
     * @param observer Instance of the observer
     */
    virtual void subscribe(E event, IObserver<S,E>* observer) = 0;
    /**
     * @brief Unsubscribes an observer from the current subject for a specific event
     * @param event The event to listen
     * @param observer Instance of the observer
     */
    virtual void unsubscribe(E event, IObserver<S,E>* observer) = 0;
    
    /**
     * @brief Notifies all subscribed observers when a specific event occurs.
     * @param event The event triggered by the subject.
     */
    virtual void notify(E event) = 0;
};

#endif