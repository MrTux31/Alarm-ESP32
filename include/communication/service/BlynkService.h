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

    void init(const char* authToken);

    void update() override;

    bool isConnected() override;

    void onAction(String key, std::function<void(String)> callback) override;

    void sendData(String key, String data) override;
    
    void triggerAction(int action, String value);

    /**
     * Docstring
     */
    void onConnect(std::function<void()> callback) override;

    void pushNotification(String eventCode, String description) override;

    /**
     * Docstring
     */
    void triggerConnect();

private:
    std::map<int, std::function<void(String)>> _actions;

    int getVirtualPin(String key);

    //Called function when the service is online / back online
    std::function<void()> _connectCallback;
};

#endif