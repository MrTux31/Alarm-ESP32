#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <Arduino.h>
#include <service/ICommunicationService.h>
#include <AlarmManager.h>
#include <IObserver.h>
#include <WiFi.h>
#include <ezTime.h>
#include <map>
#include <queue>
#include <service/INotificationService.h>
#include <NotificationQueue.h>
class CommunicationManager : public IObserver<AlarmManager, AlarmManagerEvent>, public IObserver<DetectionLoop, LoopEvent>{

public:
    /**
     * @struct Config
     * @brief Configuration map for binding network topics/pins to specific states.
     * 
     * This structure decouples the manager from hardcoded strings or protocol-specific keys.
     * It allows the exact same C++ logic to work with Blynk virtual pins (e.g., "V1", "ON")
     * or MQTT topics (e.g., "home/alarm/cmd", "armed_away") via Dependency Injection.
     */
    struct Config {
        
        /** @struct Keys
         *  @brief Holds the network identifiers (Blynk virtual pins, MQTT topics, or REST URLs)
         */
        struct Keys {
            String status;      // Unified display string channel (e.g., "V0")
            String armDesarm;   // Arming/Disarming command toggle (e.g., "V1")
            String manualMode;  // Manual siren override toggle (e.g., "V3")
            String triggeredLoop; //Logs section to track opened loops
            String triggeredLoopNotification; //Event code associated to triggered loops (used to send push notifications)
        } keys;

        /** @struct Values
         *  @brief Holds the expected raw text values sent or received over the network stack
         */
        struct Values {
            String arm;           // Message triggering an ARM command (e.g., "ON")
            String disarm;        // Message triggering a DISARM command (e.g., "OFF")
            String manualModeOn;  // Message triggering the physical siren ON
            String manualModeOff; // Message triggering the physical siren OFF
        } values;
    };

    CommunicationManager(ICommunicationService& commService, const Config& config, AlarmManager& alarmManager);

    void init();

    void update();

    //Method for observer pattern
    /**
     * TODO : Docstring
     */
    void update(AlarmManager* subject, AlarmManagerEvent event) override;

    /**
     * TODO : Docstring
     */
    void update(DetectionLoop* subject, LoopEvent event) override;

    void setNotificationService(INotificationService& notifier);

    void setPosix(String posixRule);
private:
    //An instance of a Communication Service used
    ICommunicationService& _communicationService; 
    Config _config;
    AlarmManager& _alarmManager;
    INotificationService* _notifier;
    NotificationQueue _notifQueue;

    Timezone _timeZone;

    //Opened loops logging
    DetectionLoop* _lastTriggeredLoop = nullptr;

    /**
     * TODO : Docstring
     */
    void syncAll();

    void syncAlarmState();

    void syncArmDesarm();

    void syncManualMode();

    void syncTriggeredLoop();

};

#endif