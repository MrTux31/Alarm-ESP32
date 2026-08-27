#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

/**
* @class WiFiManager
* @brief Singleton for cleanly managing the WiFi connection.
* It implements automatic reconnection logic in the event that WiFi access is lost.
*/
class WiFiManager{

private:

    WiFiManager() = default;
    //Singleton instance
    static WiFiManager* _instance;
    const char* _ssid;
    const char* _password;

    unsigned long _previousRetry = 0;
    const unsigned long RETRY_INTERVAL = 30000;
    

    /**
     * @brief Captures the disconnection reason; if it is not an issue with credentials,
     * the methods keeps in mind that disconnection happened to retry soon.
     */
    static void onWifiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onWifiConnected(WiFiEvent_t event, WiFiEventInfo_t info);


    bool _wasDisconnected;

public:

    static WiFiManager* getInstance();
    
    void init(const char* ssid, const char* password);

    /**
    * @brief Updates the manager, tries automatically to reconnect if 
    * a disconnection  has been detected.
    * Must be called continuously in the main loop.
    */
    void update();

    /**
    * @brief Retries to reconnect to the wifi 
    * if the waiting period between each attempt has been reached.
    */
    void retryConnection();
    
    
    

};


#endif