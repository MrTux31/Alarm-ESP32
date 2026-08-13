#include <CommunicationManager.h>
CommunicationManager::CommunicationManager(ICommunicationService& commService, const Config& config, AlarmManager& alarmManager) :
_communicationService(commService),_config(config),_alarmManager(alarmManager){

}

void CommunicationManager::init(){

    //Configuring the distant arming / disarming action
    _communicationService.onAction(_config.keys.armDesarm, [this](String value){
        if(value == _config.values.arm){ //If the order received is to arm the alarm
            _alarmManager.armAlarm();
        }
        else if(value == _config.values.disarm){
            _alarmManager.disarmAlarm();
        }
        
    });

    //Configuring the distant manual mode action
    _communicationService.onAction(_config.keys.manualMode, [this](String value){
        if(value == _config.values.manualModeOn){
            _alarmManager.triggerSirenManually(true);
        }
        else if(value == _config.values.manualModeOff){
            _alarmManager.triggerSirenManually(false);
        }   
    });

    //Subscribing to the alarm manager's events :
    _alarmManager.subscribe(ALARM_ARMING, this);
    _alarmManager.subscribe(ALARM_ARMED, this);
    _alarmManager.subscribe(ALARM_DISARMED, this);
    _alarmManager.subscribe(ALARM_INTRUSION, this);
    _alarmManager.subscribe(ALARM_MANUAL_ON, this);
    _alarmManager.subscribe(ALARM_MANUAL_OFF, this);

    // Triggered on initial startup or after a connection loss.
    // Overwrites outdated server data with the ESP32's current physical state.
    _communicationService.onConnect([this](){
        syncAll();
    });

    //Subscribing to each loop of the manager to log when they are opened
    for(DetectionLoop* loop : _alarmManager.getAllLoops()){
        loop->subscribe(TRIGGERED, this);
         loop->subscribe(PHYSICALLY_OPEN, this);
    }
}

void CommunicationManager::update(){
    _communicationService.update();
    processPendingNotifications();
}


void CommunicationManager::processPendingNotifications(){
    //If there are pending notifications, sending them
    for(auto& it : _pendingNotifications){ //iterating over each event code
        String eventCode = it.first; //Getting the code
        std::queue<String>& notifications = it.second; //Getting the queue associated to this code
        if(!notifications.empty()){
            if(canSendNotificationNow()){ //Antiflood delay respected + connection working
                String notif = notifications.front();
                _notifier->pushNotification(eventCode, notif);
                notifications.pop(); 
                _lastNotificationSentAt = millis();
                Serial.println("[Queue] Notification sent.");
                return;
                
            }
        }
    }
}

//This method is called when a change is detected in the alarm manager
void CommunicationManager::update(AlarmManager* subject, AlarmManagerEvent event){
    syncAlarmState();
    syncArmDesarm();
    syncManualMode();
}

//This method is called when a change is detected in a subscribed loop
void CommunicationManager::update(DetectionLoop* subject, LoopEvent event){
    //Variables for the notification
    String notifCode = _config.keys.triggeredLoopNotification;
    String message = "Zone : "+ subject->getName();
  
    if(event == TRIGGERED){
        if(_alarmManager.getCurrentState() == AlarmManager::ARMED || _alarmManager.getCurrentState() == AlarmManager::INTRUSION){
            sendNotification(notifCode, message);
            String log = logOpenedLoop(subject);
            syncOpenedLoops(); 
        }
    }
    
}

    

void CommunicationManager::syncAll(){
    syncAlarmState();
    syncArmDesarm();
    syncManualMode();
    syncOpenedLoops();
    
}

void CommunicationManager::syncAlarmState(){
    //Sync global alarm Status
    switch (_alarmManager.getCurrentState()){
        //Informs the communication service that the alarm is currently arming
        case AlarmManager::ARMING:
            _communicationService.sendData(_config.keys.status, "Alarme en cours d'armement...");
            break;
        
        case AlarmManager::ARMED:
            _communicationService.sendData(_config.keys.status, "Alarme armée");
            break;

        case AlarmManager::DISARMED:
            _communicationService.sendData(_config.keys.status, "Alarme désarmée");
            break;

        
        case AlarmManager::INTRUSION:
        case AlarmManager::STANDBY:{ //When standby, it's considered as intrusion (every loop are opened)
            std::vector<DetectionLoop*> faultyLoops = _alarmManager.getTriggeredLoops();
            String names = "";
            //Listing opened loops names
            for(DetectionLoop *loop : faultyLoops){
                names += loop->getName() + "\n";
            }
            _communicationService.sendData(_config.keys.status, "INTRUSION EN COURS :\n" + names);
            break;
        }
    }
}

void CommunicationManager::syncArmDesarm(){
    //Sync the Arm / Disarm button
    if(_alarmManager.getCurrentState() == AlarmManager::DISARMED ){
        _communicationService.sendData(_config.keys.armDesarm, _config.values.disarm);
    }else{
        _communicationService.sendData(_config.keys.armDesarm, _config.values.arm);
    }

}

void CommunicationManager::syncManualMode(){
    //Sync the manual siren trigger button
    if(_alarmManager.isSirenTriggeredManually()){
        _communicationService.sendData(_config.keys.manualMode, _config.values.manualModeOn);
    }else{
        _communicationService.sendData(_config.keys.manualMode, _config.values.manualModeOff);
    }

}

void CommunicationManager::syncOpenedLoops(){
    if (_openedLoopsLog.empty()) return;

    //Generating full history
    String fullHistory = "";
    for(String loopLog : _openedLoopsLog){
        fullHistory += loopLog +"\n";;
    }
    //Sending full history in one block
    _communicationService.sendData(_config.keys.logs, fullHistory);
    _openedLoopsLog.clear();
}

String CommunicationManager::logOpenedLoop(DetectionLoop* loop){
    if(_openedLoopsLog.size() >= MAX_LOOPS_LOG){
            //Remove oldest log
            _openedLoopsLog.erase(_openedLoopsLog.begin());
        }
    String timeString = _timeZone.dateTime("H:i:s"); 
    String logMessage = "[" + timeString + "] Zone : " + loop->getName() + " ouverte.";
    //Saving the new log
    _openedLoopsLog.push_back(logMessage); 

    return logMessage;
}

void CommunicationManager::setNotificationService(INotificationService& notifier){
    _notifier = &notifier;
}
void CommunicationManager::sendNotification(String code, String desc){
    if(hasNotificationService()){
        //Adding the notification to the pending notifications map
        _pendingNotifications[code].push(desc);
        Serial.println("[Queue] Notification added to the queue");

    }
}

bool CommunicationManager::hasNotificationService(){
    return _notifier != nullptr;
}
bool CommunicationManager::canSendNotificationNow(){

    return _communicationService.isConnected() && millis() - _lastNotificationSentAt >= NOTIFICATION_SENDING_DELAY;
}

void CommunicationManager:: setPosix(String posixRule){
    _timeZone.setPosix(posixRule);
}