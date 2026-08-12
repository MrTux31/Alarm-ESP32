#ifndef I_COMMUNICATION_SERVICE_H
#define I_COMMUNICATION_SERVICE_H
#include <Arduino.h>

/**
 * @class ICommunicationService
 * @brief Universal interface for bidirectional network communication with a remote server or machine.
 */
class ICommunicationService{ //Interface

public:
    virtual ~ICommunicationService(){};

    /**
     * @brief Keeps the network pile active. Must be called continuously in the main loop.
     */
    virtual void update() = 0;

    /**
     * @brief Returns true if the connection with the server is active and working 
     * correctly.
     */
    virtual bool isConnected() = 0;

    /**
     * @brief Sends data to the remote server based on a universal string key.
     */
    virtual void sendData(String key, String data) = 0;
    
    /**
     * @brief Binds a network key event to a specific software action callback.
     */
    virtual void onAction(String key, std::function<void(String)> callback) = 0;

    /**
     * TODO : docstring
     */
    virtual void onConnect(std::function<void()> callback) = 0;



};


#endif