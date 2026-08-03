#ifndef DETECTION_LOOP_H
#define DETECTION_LOOP_H
#include <Arduino.h>
class DetectionLoop
{
private:
    int _pin;
    String _name;
    uint8_t _mode;
    bool _isEnabled = true; // True if the loop is active, false if it is bypassed/excluded
    
    using TriggerCallback = std::function<void(DetectionLoop*)>;
    TriggerCallback _onTriggerCb = nullptr; //The pointer to the callback function

    unsigned long _delayMs; //Wait time before considering the loop truly open
    unsigned long _openedSince; //Timestamp when the loop was physically opened
    bool _wasOpened; //Indicates whether the loop was open during the previous check

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
     * @brief Registers a callback function to be executed when the loop is triggered.
     * 
     * @param callback The function to execute, passing a pointer to this DetectionLoop.
     */
    void onTrigger(TriggerCallback callback);

    /**
     * Detects the physical opening of the loop 
     * and records the time of the opening
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
     * Disables and bypasses the detection loop.
     * 
     * Stops the loop from scanning its physical sensor. Useful for auto-excluding 
     * a triggered or faulty zone, allowing the rest of the system to re-arm safely.
     */
    void disable();

    /**
     * Enables the detection loop for active monitoring.
     * 
     * Reactivates the loop so the AlarmManager can process its sensor data again.
     * Typically called during a global system reset or when disarming.
     */
    void enable();

    /**
     * Checks whether the detection loop is currently active.
     * 
     * @return true if the loop is enabled and monitoring, false if it is bypassed.
     */
    bool isEnabled() const;

    /**
     * Sets the timeout period before the loop
     * is considered open.
     */
    void setDelay(unsigned long delayMs);

    /**
     * Determines whether the loop is considered open, taking into account
     * any potential wait time.
     */
    bool isTriggered();

    /**
     * Allows the loop state to be 
     * reset after it has been triggered.
     */
    void resetTrigger();


    /**
     * Check if the loop has been opened physically only.
     */
    bool isPhysicalOpen();

    String getName();
    void setName(String name);

    


};

#endif
