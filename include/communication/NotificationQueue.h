#ifndef NOTIFICATION_QUEUE_H
#define NOTIFICATION_QUEUE_H

#include <Arduino.h>
#include <service/INotificationService.h>
#include <queue>
/**
 * Class designed to buffer notifications in a queue, enforcing 
 * a mandatory anti-flood dispatch delay and validating network 
 * connectivity before transmission.
 */
class NotificationQueue{
private:
    struct Notification{
        String code;
        String desc;
    };
    INotificationService* _notifier = nullptr;
    std::queue<Notification> _pendingNotifications;
    const int MAX_NOTIFICATIONS = 20; //Max pending notifications in queue
    const unsigned long NOTIFICATION_SENDING_DELAY = 5000; //Anti flood protection
    unsigned long _lastNotificationSentAt = 0;

public: 
    NotificationQueue() = default;

    NotificationQueue(INotificationService& _notifier);

    void update(bool networkConnected);

    /**
     * @brief Safely enqueues a new notification message into the background buffer.
     * 
     * This method acts as a RAM firewall. If the number of pending alerts reaches 
     * MAX_NOTIFICATIONS, it automatically invokes a FIFO (First In, First Out) replacement: 
     * the oldest alert sitting at the front of the queue is permanently dropped from RAM 
     * to allocate memory for the incoming fresh event.
     * 
     * @param code The target event code or virtual channel for the push alert.
     * @param desc The payload message string detailing the sensor event.
     */
    void pushNotification(String code, String desc);

    void setNotifier(INotificationService& notifier);
};


#endif
