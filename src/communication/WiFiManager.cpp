#include <WiFiManager.h>
#include <WiFi.h>
WiFiManager* WiFiManager::_instance = nullptr;

WiFiManager* WiFiManager::getInstance(){
    if(_instance == nullptr){
        _instance = new WiFiManager();
    }
    return _instance;
}

void WiFiManager::init(const char* ssid,const char* password){
    _ssid = ssid;
    _password = password;
    WiFi.begin(ssid, password);
    _previousRetry = millis();
    WiFi.onEvent(onWifiDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}

void WiFiManager::update(){
    //If the wifi managed to reconnect
    if(WiFi.status() == WL_CONNECTED){
        _wasDisconnected = false;
        return;
    }
    
    if(_wasDisconnected){
        retryConnection();
    }
}

void WiFiManager::retryConnection(){
    if(millis()- _previousRetry >= RETRY_INTERVAL){
        
        WiFi.begin(_ssid, _password);
        //It will be true again with the onWifiDisconnected() method if connection fails
        //Setting it to `false` prevents spamming the function call.
        _wasDisconnected = false; 
        _previousRetry = millis();

    }
}

void WiFiManager::onWifiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
    uint8_t reason = info.wifi_sta_disconnected.reason;
    //Wrong identifiants
    if(reason == WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT){
        WiFi.disconnect();
        return; //We don't retry
    }
    //Get singleton instance
    WiFiManager* instance = WiFiManager::getInstance();
    instance->_wasDisconnected = true;
    instance->_previousRetry = millis();

}