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
     * Initialise la connexion à Blynk avec le 
     * token d'identification de l'utilisateur.
     */
    void init(const char* authToken);

    void update() override;

    bool isConnected() override;

    /**Permet de lier une broche virtuelle de Blynk à une fonction callback
     */
    void onAction(String key, std::function<void(String)> callback) override;

    /**
     * Permet d'envoyer des informations sur une broche virtuelle de blynk
     */
    void sendData(String key, String data) override;
    
    /**
     * Permet de déclencher des actions provenenant de Blynk
     */
    void triggerAction(int action, String value);

    
    void onConnect(std::function<void()> callback) override;

    /**
     * Permet de log un évènement sur blynk
     * (Blynk se charge de : envoi notif sur téléphone, mail...)
     */
    void pushNotification(String eventCode, String description) override;

    /**
     * Permet de déclencher le callback de connexion enregistré
     * avec onConnect()
     */
    void triggerConnect();

private:
    std::map<int, std::function<void(String)>> _actions;

    /**
     * Permet de convertir une broche blynk qui est en String
     * vers un entier.
     * Se charge automatiquement du parsing.
     */
    int getVirtualPin(String key);

    //Called function when the service is online / back online
    std::function<void()> _connectCallback;
};

#endif