#ifndef I_BLYNK_SERVICE_H
#define I_BLYNK_SERVICE_H

#include <Arduino.h>
#include <map>
#include <functional>
#include <service/ICommunicationService.h>
#include <service/INotificationService.h>

/**
 * @class Generic class for communicating
 * with Blynk and performing actions.
 * This class implements the interface ICommunicationService
 * and INotificationService
 */
class BlynkService : public ICommunicationService, public INotificationService{

public: 

    BlynkService();

    /**
     * @brief Initializes the connection to Blynk using the 
     * user's authentification token.
     */
    void init(const char* authToken);

    void update() override;

    bool isConnected() override;

   /**
    * @brief Links a Blynk virtual pin to a callback function
    */
    void onAction(String key, std::function<void(String)> callback) override;

   /**
     * @brief Sends information to a Blynk virtual pin
     */
    void sendData(String key, String data) override;
    
    /**
     * @brief Allows triggering actions coming from Blynk
     */
    void triggerAction(int action, String value);

    
    void onConnect(std::function<void()> callback) override;

    /**
     * @brief Logs an event to Blynk
     * (Blynk handles sending notifications to the phone, email, etc.)
     */
    void pushNotification(String eventCode, String description) override;

    /**
     * @brief Triggers the connection callback registered
     * with onConnect()
     */
    void triggerConnect();

private:
    /**    
    * @brief Map linking Blynk virtual pins to callback functions to be called
    * when commands from Blynk are received
    */
    std::map<int, std::function<void(String)>> _actions;

    /**
     * @brief Converts a Blynk pin (provided as a String)
     * into an integer.
     * Automatically handles the parsing.
     */
    int getVirtualPin(String key);

    //Called function when the service is online / back online
    std::function<void()> _connectCallback;
};

#endif