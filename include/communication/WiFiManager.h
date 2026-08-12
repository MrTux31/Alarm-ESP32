#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
class WiFiManager{

private:

    WiFiManager() = default;
    static WiFiManager* _instance;
    const char* _ssid;
    const char* _password;

    unsigned long _previousRetry = 0;
    const unsigned long RETRY_INTERVAL = 30000;
    

    /**
     * Tries a reconnection to the wifi
     */
    static void onWifiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

    bool _wasDisconnected;

public:

    static WiFiManager* getInstance();
    
    void init(const char* ssid, const char* password);

    void update();

    void retryConnection();
    
    
    

};


#endif