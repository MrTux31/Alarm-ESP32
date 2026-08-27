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
    _previousRetry = millis();
    WiFi.begin(ssid, password);
    WiFi.onEvent(onWifiDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(onWifiConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
}

void WiFiManager::update(){
    //If the wifi managed to reconnect
    if(WiFi.status() == WL_CONNECTED && _wasDisconnected){
        Serial.println("[WifiManager] Wifi re-connected successfully");
        _wasDisconnected = false;
        return;
    }
    
    if(WiFi.status() != WL_CONNECTED && _wasDisconnected){
        retryConnection();
    }
}

void WiFiManager::retryConnection(){
    if(millis()- _previousRetry >= RETRY_INTERVAL){
        Serial.println("[WifiManager] Retrying to connect");
        _previousRetry = millis();
        WiFi.begin(_ssid, _password);
        
    }
}

void WiFiManager::onWifiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
    uint8_t reason = info.wifi_sta_disconnected.reason;
    //Wrong identifiants
    if(reason == WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT){
        WiFi.disconnect();
        Serial.println("[WifiManager] Wifi disconnected");
        return; //We don't retry
    }
    //Get singleton instance
    WiFiManager* instance = WiFiManager::getInstance();
    
    if (!instance->_wasDisconnected) {
        Serial.println("[WifiManager] Wifi disconnected");
        //Set to true, reconnection will be attempted on the next call to the update() method.
        instance->_wasDisconnected = true;
    }

}

void WiFiManager::onWifiConnected(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println("[WifiManager] Wifi connected");

}