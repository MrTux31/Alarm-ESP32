#include <CommunicationManager.h>

CommunicationManager::CommunicationManager(ICommunicationService& commService, Config config, AlarmManager& alarmManager) :
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
}

void CommunicationManager::update(){
    _communicationService.update();
}

//This method is called when a change is detected in the alarm manager
void CommunicationManager::update(AlarmManager* subject, AlarmManagerEvent event){
    syncAll();
}

void CommunicationManager::syncAll(){
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

        case AlarmManager::INTRUSION:{
            std::vector<DetectionLoop*> faultyLoops = _alarmManager.getTriggeredLoops();
            String names = "";
            //Listing opened loops names
            for(DetectionLoop *loop : faultyLoops){
                names += loop->getName() + ", ";
            }
            _communicationService.sendData(_config.keys.status, "INTRUSION EN COURS : " + names);
            break;
        }
    }
    //Sync the Arm / Disarm button
    if(_alarmManager.getCurrentState() == AlarmManager::DISARMED ){
        _communicationService.sendData(_config.keys.armDesarm, _config.values.disarm);
    }else{
        _communicationService.sendData(_config.keys.armDesarm, _config.values.arm);
    }

    //Sync the manual siren trigger button
    if(_alarmManager.isSirenTriggeredManually()){
        _communicationService.sendData(_config.keys.manualMode, _config.values.manualModeOn);
    }else{
        _communicationService.sendData(_config.keys.manualMode, _config.values.manualModeOff);
    }

}


