#include "service/BlynkService.h"
#include "BlynkSimpleEsp32.h"

//Used to store the class instance
static BlynkService* instancePtr = nullptr; 

BlynkService::BlynkService(){
    //Storing the instance to use it with BLYNK_WRITE_DEFAULT()
    instancePtr = this; 
}

void BlynkService::init(const char* authToken){
    Blynk.config(authToken);
}

void BlynkService::update(){
    if(WiFi.status() == WL_CONNECTED){
        Blynk.run();
    }
}

void BlynkService::onAction(String key, std::function<void(String)> callback){
    int pin = getVirtualPin(key);
    _actions[pin] = callback;
}


void BlynkService::sendData(String key, String data){
    int pin = getVirtualPin(key);
    //Sending data to blynk
    if(WiFi.status() == WL_CONNECTED){
        Blynk.virtualWrite(pin, data);
    }
}

void BlynkService::onConnect(std::function<void()> callback){
    _connectCallback = callback;
}

int BlynkService::getVirtualPin(String key){
    int pin = key.toInt();
    //If not int trying to remove the "V" from "V1", "V2" etc (standard for blynk virtual pins)
    if(pin == 0 && key != "0"){
        pin = key.substring(1).toInt();
    }
    return pin;
}

void BlynkService::triggerAction(int action, String value){
    auto it = _actions.find(action);
    //If the action exists and the callback is not nullptr
    if(it != _actions.end() && it->second != nullptr){ 
        it->second(value); //Callback invocation
    }
}

void BlynkService::triggerConnect(){
    if(_connectCallback != nullptr){
        _connectCallback();
    }
}

//Actions coming from Blynk to the device
BLYNK_WRITE_DEFAULT(){
    if(instancePtr != nullptr){
        //Triggering the callback
        instancePtr->triggerAction(request.pin, param.asStr());
    }
}

//Executed when connection to Blynk is established (for first connection or when connection is back after a loss)
BLYNK_CONNECTED(){
    if(instancePtr != nullptr){
        //Triggering the callback
        instancePtr->triggerConnect();
    }
}