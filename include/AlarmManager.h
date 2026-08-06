
#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H
#include <Arduino.h>
#include <vector>
#include <DetectionLoop.h>
#include <ISiren.h>
#include <constants.h>
#include <IObserver.h>

enum AlarmManagerEvent { ALARM_ARMED, ALARM_ARMING, ALARM_DISARMED, ALARM_INTRUSION };

class AlarmManager : public ISubject<AlarmManager,AlarmManagerEvent> ,IObserver<DetectionLoop,LoopEvent>{

public:
    enum SystemState { DISARMED, ARMING, ARMED, INTRUSION, STANDBY };

    AlarmManager(ISiren& siren ,std::vector<DetectionLoop*> &loops, unsigned long sirenDurationMs = DEFAULT_SIREN_DURATION_MS); 

    /**
     * Initializes each component of the alarm
     */
    void init();

    void update();
    
    /**
     * Arms the alarm system.
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

    bool isSirenTriggeredManually();

    bool isSirenActive();

    /**
     * @brief Retrieves all loops that were triggered during the current intrusion
     */
    std::vector<DetectionLoop*> getTriggeredLoops();

    /** 
    * @brief Sets the siren/alarm duration in milliseconds.
    * @param durationMs The duration in milliseconds.
    */
    void setSirenDuration(unsigned long durationMs);

    void setArmingDelay(unsigned long delayMs);
    
    void setEntryDelay(unsigned long delayMs);

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
    bool canRearmAlarm();

    //Method for Observer 
    void notify(AlarmManagerEvent event) override;
    


};

#endif