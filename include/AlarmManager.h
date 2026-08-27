
#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H
#include <Arduino.h>
#include <vector>
#include <devices/DetectionLoop.h>
#include <ISiren.h>
#include <constants.h>
#include <IObserver.h>

enum AlarmManagerEvent { ALARM_ARMED, ALARM_ARMING, ALARM_DISARMED, ALARM_INTRUSION, ALARM_STANDBY, ALARM_MANUAL_ON, ALARM_MANUAL_OFF };

/**
 * @class AlarmManager
 * @brief Represents the complete alarm system core logic.
 * 
 * @details Responsible for managing all alarm logic, states, and timing.
 *          Implements the following interfaces:
 *          - ISubject<AlarmManager,AlarmManagerEvent> to notify communication/log services of system events.
 *          - IObserver<DetectionLoop,LoopEvent> to monitor physical detection loops.
 */
class AlarmManager : public ISubject<AlarmManager,AlarmManagerEvent> ,public IObserver<DetectionLoop,LoopEvent>{

public:
    enum SystemState { DISARMED, ARMING, ARMED, INTRUSION, STANDBY };

    /**
     * @brief Constructor
     * @param siren The instance of the Siren used in the alarm system
     * @param loops A vector of the detection loops used in the alarm
     * @param sirenDurationMs The duration of the siren in millisencods
     */
    AlarmManager(ISiren& siren ,std::vector<DetectionLoop*> &loops, unsigned long sirenDurationMs = DEFAULT_SIREN_DURATION_MS); 

    /**
     * @brief Initializes each component of the alarm (siren, loops)
     * and subscribes to the loops (observer pattern)
     */
    void init();

    /**
     * @brief Updates the Alarm System.
     * Changes the alarm behavior based on detected events.
     * Must be called in the main loop.
     */
    void update();

    /**
     * @brief Allows toggling the alarm state while notifying its observers.
     */
    void toggleState(SystemState newState);

    /**
     * @brief Arms the alarm system.
     */
    void armAlarm();

   /**
     * @brief Disarms the system and performs a full security reset.
     * 
     * Stops the siren, changes the state to DISARMED, clears the event memory 
     * of all loops, and reenables any previously bypassed (disabled) zones 
     * to prepare for the next arming cycle.
     */
    void disarmAlarm();

     /**
     * @brief Manually overrides and forces the siren state.
     * 
     * Allows the user to toggle the siren on or off independently of the system state.
     * When turned on manually, the siren sounds indefinitely until the user explicitly stops it.
     * 
     * @param turnOn True to activate the siren manually, false to deactivate it.
     */
    void triggerSirenManually(bool turnOn);

    /**
     * @brief Returns true if the siren is currently triggered manually
     */
    bool isSirenTriggeredManually();

    /**
     * @brief Returns true if the siren is currently active
     */
    bool isSirenActive();

    /**
     * @brief Retrieves all loops that were triggered during the current intrusion
     */
    std::vector<DetectionLoop*> getTriggeredLoops();

    /**
     * @brief Returns alarm's loops
     */
    std::vector<DetectionLoop*>& getAllLoops();

    /** 
    * @brief Sets the siren/alarm duration in milliseconds.
    * @param durationMs The duration in milliseconds.
    */
    void setSirenDuration(unsigned long durationMs);

    /** 
    * @brief Sets the delay in milliseconds that the alarm is taking to be armed.
    * @param durationMs The duration in milliseconds.
    */
    void setArmingDelay(unsigned long delayMs);
    
    /**
     * @brief Allows to set the delay before the alarm detects an intrusion. 
     * This gives the homeowner a few seconds to disarm the alarm upon returning home.
     * @param delayMs The delay in milliseconds
     */
    void setEntryDelay(unsigned long delayMs);

    /**
     * @brief Returns the current state of the alarm system
     */
    SystemState getCurrentState();
    
    //Overrided method for the Subject
    void update(DetectionLoop* loop, LoopEvent event) override;

    //Overrided methods for the Observer/////
    void subscribe(AlarmManagerEvent event, IObserver<AlarmManager, AlarmManagerEvent>* observer) override;
    void unsubscribe(AlarmManagerEvent event, IObserver<AlarmManager, AlarmManagerEvent>* observer) override;
    

private:
     //Hash maps for observers
    std::unordered_map<AlarmManagerEvent, std::vector<IObserver<AlarmManager, AlarmManagerEvent>*>> observers;

    ISiren& _siren; //reference to the implementation of ISiren
    SystemState _state; // The current state of the alarm
    bool _intrusionDetectedThisCycle;
    bool _isSirenTriggeredManually = false; //If the user manually triggers the siren

    std::vector<DetectionLoop*> &_loops; //Detection loops for the alarm

    unsigned long _sirenDurationMs;  //The maximum duration for the siren (when not triggered manually)
    unsigned long _intrusionStartedAt ; //The moment the intrusion began and the siren turned active

    unsigned long _armingDelayMs = 0; //Waiting time before the alarm is armed,
                             //allowing the owner to leave
    
    unsigned long _armingSince = 0; //The moment the alarm arming process began

     /**
     * @brief Clears the event memory of all remaining active zones.
     * 
     * Resets the internal triggers and timestamps for all loops without affecting 
     * their bypass status. This ensures that non-faulty zones start fresh 
     * when the system automatically returns to surveillance.
     */
    void resetAlarm(); 

    /**
     * @brief Initiates the intrusion alert sequence.
     * 
     * Activates the siren, records the precise start time of the event,
     * and prepares the system to monitor the maximum legal duration of the alarm.
     */
    void beginIntrusion();

    /**
     * @brief Refreshes sensor states and processes auto-recovery for all loops.
     * 
     * Iterates through the entire collection of detection loops to scan physical 
     * inputs and automatically re-enable any secured zones.
     */
    void updateAllLoops();

    /**
     * @brief Check if the alarm can be re-armed, it means that there is
     * at least one remaining active loop.
     * @returns true if the Alarm is allowed to be re-armed
     */
    bool canRearmAlarm();

    //Method for Observer pattern 
    void notify(AlarmManagerEvent event) override;
    


};

#endif