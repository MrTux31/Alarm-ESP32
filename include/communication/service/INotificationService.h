#ifndef I_NOTIFICATION_SERVICE_H
#define I_NOTIFICATION_SERVICE_H
#include <Arduino.h>

/**
 * @class INotificationService
 * @brief A simple interface to send a notifications
 */
class INotificationService{

public:
    virtual ~INotificationService(){};

    /**
     * @brief Sends a notification associated to an event code
     */
    virtual void pushNotification(String eventCode, String description) = 0;


};



#endif