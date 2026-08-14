#include <NotificationQueue.h>

NotificationQueue::NotificationQueue(INotificationService& notifier) : 
_notifier(&notifier){

}


void NotificationQueue::pushNotification(String code, String desc){
    //We dont add notification in queue if no notification service
    if(_notifier == nullptr){return;} 

    //If the limit has been reached
    if(_pendingNotifications.size() >= MAX_NOTIFICATIONS){
        //Removing the oldest one in queue
        _pendingNotifications.pop(); 
    }
    _pendingNotifications.push({code, desc});
    Serial.println("[Queue] Notification added to the queue");

}
void NotificationQueue::update(bool isNetworkConnected){
    if(!isNetworkConnected){return;}
    if(_notifier == nullptr){ return; } 
    if(_pendingNotifications.empty()){return;}

    //Anti flood protection
    if(millis()- _lastNotificationSentAt >= NOTIFICATION_SENDING_DELAY){
        Notification notif = _pendingNotifications.front();
        //Sending notification through the notifier
        _notifier->pushNotification(notif.code, notif.desc);
        _pendingNotifications.pop(); 
        _lastNotificationSentAt = millis();
        Serial.println("[Queue] Notification sent.");
    }
}
  
void NotificationQueue::setNotifier(INotificationService& notifier) {
        _notifier = &notifier;
}

