#ifndef I_BLYNK_SERVICE_H
#define I_BLYNK_SERVICE_H

#include <Arduino.h>
#include <map>
#include <functional>
#include <ICommunicationService.h>

/**
 * @class Generic class for communicating
 * with Blynk and performing actions.
 * This class implements the interface ICommunicationService
 */
class BlynkService : public ICommunicationService{

public: 

    BlynkService();

    void init(const char* authToken);

    void update() override;

    void onAction(String key, std::function<void(String)> callback) override;

    void sendData(String key, String data) override;
    
    void triggerAction(int action, String value);

private:
    std::map<int, std::function<void(String)>> _actions;

    int getVirtualPin(String key);

};

#endif