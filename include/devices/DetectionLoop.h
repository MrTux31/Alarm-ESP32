#ifndef DETECTION_LOOP_H
#define DETECTION_LOOP_H
#include <Arduino.h>
#include <ISubject.h>
#include <unordered_map>
#include <vector>
#include <IObserver.h>

enum LoopEvent{
    TRIGGERED,
    PHYSICALLY_OPEN
};
/**
 * @class DetectionLoop 
 * @brief Class representing an alarm detection loop.
 * Implements the ISubject interface for the Observer pattern.
 */
class DetectionLoop : public ISubject<DetectionLoop,LoopEvent>{
private:
    //Hash maps for observers
    std::unordered_map<LoopEvent, std::vector<IObserver<DetectionLoop, LoopEvent>*>> observers;

    int _pin;
    String _name;
    uint8_t _mode;
    bool _isEnabled = true; // True if the loop is active, false if it is bypassed/excluded
    
    unsigned long _delayMs; //Wait time before considering the loop truly open
    unsigned long _openedSince; //Timestamp when the loop was physically opened
    
    //Opened = Physically open loop
    //Triggered = Loop considered truly open after the time delay has been accounted for
    bool _wasOpened; //Indicates whether the loop was open during the previous check
    bool _wasTriggered;

    //Method for observer pattern
    void notify(LoopEvent event) override;

public:
    /** 
    * Class constructor
    * @param pin : The pin to which the loop is connected
    * @param name : The name associated with the loop
    * @param delayMs : The wait time before the loop is considered open
    * @param mode : The pin's startup mode based on the loop sensor
    */

    DetectionLoop(int pin, String name, unsigned long delayMs = 0 ,uint8_t mode = INPUT_PULLUP);
    
    void init();

    /**
     * @brief Detects the physical opening of the loop 
     * and records the time of the opening
     * Must be called continuously in the main loop.
     */
    void update();

    /**
     * @brief Automatically re-enables the loop as soon as the physical sensor returns to a closed (secured) state.
     * 
     * This method is used for auto-exclusion (bypass) recovery, checking if a previously 
     * disabled zone has been safely closed before the system re-arms.
     */
    void tryAutoReenable();

    /**
     * @brief Disables and bypasses the detection loop.
     * 
     * Stops the loop from scanning its physical sensor. Useful for auto-excluding 
     * a triggered or faulty zone, allowing the rest of the system to re-arm safely.
     */
    void disable();

    /**
     * @brief Enables the detection loop for active monitoring.
     * 
     * Reactivates the loop so the AlarmManager can process its sensor data again.
     * Typically called during a global system reset or when disarming.
     */
    void enable();

    /**
     * @brief Checks whether the detection loop is currently active.
     * 
     * @return true if the loop is enabled and monitoring, false if it is bypassed.
     */
    bool isEnabled() const;

    /**
     * @brief Sets the timeout period before the loop
     * is considered open.
     */
    void setDelay(unsigned long delayMs);

    /**
    * @brief Returns true if the loop has a delay > 0ms 
    */
    bool isDelayed();

    /**
     * @brief Forces the detection loop to be considered as triggered
     */
    void forceTrigger();

    /**
     *@brief  Determines whether the loop is considered open, taking into account
     * any potential wait time.
     */
    bool isTriggered();

    /**
     * @brief Allows the loop state to be 
     * reset after it has been triggered.
     */
    void resetTrigger();
    
    /**
     * @brief Check if the loop has been opened physically only.
     */
    bool isPhysicalOpen();

    /**
     * @brief Get the loop's name
     * @return the name
     */
    String getName();
    
    /**
     * @brief Set the loop's name
     * @param name the name
     */
    void setName(String name);


    //Overrided methods for the observer pattern/////
    void subscribe(LoopEvent event, IObserver<DetectionLoop, LoopEvent>* observer) override;
    void unsubscribe(LoopEvent event, IObserver<DetectionLoop, LoopEvent>* observer) override;
    


};
#endif
