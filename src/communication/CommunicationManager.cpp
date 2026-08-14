#include <CommunicationManager.h>
CommunicationManager::CommunicationManager(ICommunicationService& commService, const Config& config, AlarmManager& alarmManager) :
_communicationService(commService),_config(config),_alarmManager(alarmManager)
{

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
    _alarmManager.subscribe(ALARM_STANDBY, this);
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
    }
}

void CommunicationManager::update(){
    _communicationService.update();
    _notifQueue.update(_communicationService.isConnected());

}


//This method is called when a change is detected in the alarm manager
void CommunicationManager::update(AlarmManager* subject, AlarmManagerEvent event){
    
    if(event == ALARM_DISARMED){
        //Reset the stored loop
        _lastTriggeredLoop = nullptr;
        syncTriggeredLoop();
    }
    
    switch (event){
        case ALARM_DISARMED:
        case ALARM_ARMING:
        case ALARM_ARMED:
        case ALARM_INTRUSION:
        case ALARM_STANDBY:
            syncAlarmState();
            syncArmDesarm();
            break;

        case ALARM_MANUAL_OFF:
        case ALARM_MANUAL_ON:
            syncManualMode();
            break;
        
        default:
            break;
    }
}

//This method is called when a change is detected in a subscribed loop
void CommunicationManager::update(DetectionLoop* subject, LoopEvent event){
    
    if(event == TRIGGERED){
        if(_alarmManager.getCurrentState() == AlarmManager::ARMED || _alarmManager.getCurrentState() == AlarmManager::INTRUSION){
            //Variables for the notification
            String notifCode = _config.keys.triggeredLoopNotification;
            String message = "Zone : "+ subject->getName();

            _lastTriggeredLoop = subject;
            //Sending notification
            _notifQueue.pushNotification(notifCode, message);
            syncTriggeredLoop(); 
        }
    }
    
}

void CommunicationManager::syncAll(){
    syncAlarmState();
    syncArmDesarm();
    syncManualMode();
    syncTriggeredLoop();
    
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
            _communicationService.sendData(_config.keys.status, "INTRUSION EN COURS");
            break;
        case AlarmManager::STANDBY:
            _communicationService.sendData(_config.keys.status, "En attente de boucles disponibles...");
            break;
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

void CommunicationManager::syncTriggeredLoop(){
    String logMessage;
    //No loop was opened
    if (_lastTriggeredLoop == nullptr){
        logMessage = "Aucune boucle ouverte";

    }
    else{
        String timeString = _timeZone.dateTime("H:i:s"); 
        logMessage = "[" + timeString + "] " + _lastTriggeredLoop->getName() + " ouverte.";
    }
    _communicationService.sendData(_config.keys.triggeredLoop, logMessage);

}


void CommunicationManager::setNotificationService(INotificationService& notifier){
    _notifier = &notifier;
    _notifQueue.setNotifier(notifier);
}

void CommunicationManager:: setPosix(String posixRule){
    _timeZone.setPosix(posixRule);
}