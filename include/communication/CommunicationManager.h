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

/**
 * @class CommunicationManager
 * @brief Manages the bidirectional link between the alarm system and the remote communication service.
 * 
 * @details This class acts as a mediator (using the Observer pattern). It transmits 
 * alarm and detection loop events to the server/application 
 * and executes commands received from the communication service.
 */
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

    /**
    * @brief Initializes the Communication Manager
    * - First, defines the actions to be performed for commands received from the CommunicationService in use
    * - Subscribes to events from the Alarm Manager and Detection Loops
    * - Defines the actions to be performed when the connection to the CommunicationService is established
    */
    void init();

    /**
    * @brief Updates the CommunicationService and the Internal Notification to the manager
    * Must be called continuously in the main loop.
    */
    void update();

    //Method for observer pattern
    /**
     * @brief Method called when an event is emitted by the AlarmManager.
     * Allows events to be forwarded to the CommunicationService—for example,
     * to update the alarm status (armed, disarmed, intrusion, etc.).
     */
    void update(AlarmManager* subject, AlarmManagerEvent event) override;

    /**
     * @brief Method called when an event is emitted by a DetectionLoop of the AlarmManager.
     * Allows events to be forwarded to the CommunicationService for example,
     * to send a notification when a loop is triggered.
     */
    void update(DetectionLoop* subject, LoopEvent event) override;

    /**
    * @brief Sets the notification service used to transmit notifications
    */
    void setNotificationService(INotificationService& notifier);

    /**
    * @brief Sets the time zone using a POSIX rule string.
    */
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
     * @brief Synchronizes all local alarm data with the remote communication service.
     * 
     * @details Forces a full update of the server/app state by transmitting:
     *          - Current global alarm status (arming, armed, intrusion, etc.)
     *          - Arm/disarm toggle switch state
     *          - Manual siren activation mode state
     *          - Log of the last triggered detection loop
     */
    void syncAll();

    /**
     * @brief Synchronizes the global alarm status text with the remote service.
     * 
     * @details Sends state strings such as "Alarm arming", "INTRUSION DETECTED", etc.
     */
    void syncAlarmState();

    /**
     * @brief Synchronizes the arm/disarm toggle state with the remote service.
     */
    void syncArmDesarm();

    /**
     * @brief Synchronizes the manual siren control state with the remote service.
     */
    void syncManualMode();

    /**
     * @brief Synchronizes information about the last triggered zone or loop.
     * 
     * @details Appends a timestamp to the zone name if a loop was triggered, 
     *          otherwise reports that no loops are open.
     */
    void syncTriggeredLoop();

};

#endif