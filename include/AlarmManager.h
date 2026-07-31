
#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H
#include <Arduino.h>
#include <vector>
#include <DetectionLoop.h>
#include <ISiren.h>
#include <constants.h>

enum State { DISARMED, ARMING, ARMED, ENTRY_DELAY, SIREN_ACTIVE };

class AlarmManager{

private:
    ISiren& _siren; //reference to the implementation of ISiren
    State _state; // The current state of the alarm
    std::vector<DetectionLoop> &_loops; //Detection loops for the alarm
    unsigned long _sirenDurationMs;
    unsigned long _sirenActiveSince; //The moment the siren turned active

    unsigned long _armingDelayMs = 0; //Waiting time before the alarm is armed,
                             //allowing the owner to leave
    
    unsigned long _armingSince = 0; //The moment the alarm arming process began

    /**
     * Resets the state of each loop.
     */
    void resetAlarm(); 

    /**
     * Triggers the alarm siren
     */
    void triggerSiren();
public:
    AlarmManager(ISiren& siren ,std::vector<DetectionLoop> &loops, unsigned long sirenDurationMs = DEFAULT_SIREN_DURATION_MS); 

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
     * Disarms the alarm system.
     */
    void disarmAlarm();

    bool isSirenActive();

    /* @brief Sets the siren/alarm duration in milliseconds.
    * @param durationMs The duration in milliseconds.
    */
    void setSirenDuration(unsigned long durationMs);

    void setArmingDelay(unsigned long delayMs);
    void setEntryDelay(unsigned long delayMs);

    State getCurrentState();
    
};



#endif