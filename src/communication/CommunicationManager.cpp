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


}

void CommunicationManager::update(){
    _communicationService.update();
}

void CommunicationManager::update(AlarmManager* subject, AlarmManagerEvent event){

    switch (event){

        case ALARM_ARMING:
            //Informs the communication service that the alarm is currently arming
            _communicationService.sendData(_config.keys.status, "Alarme en cours d'armement...");
            break;

        case ALARM_ARMED:
            _communicationService.sendData(_config.keys.status, "Alarme armée");
            _communicationService.sendData(_config.keys.armDesarm, _config.values.arm);
            break;

        case ALARM_DISARMED:
            _communicationService.sendData(_config.keys.status, "Alarme désarmée");
            _communicationService.sendData(_config.keys.armDesarm, _config.values.disarm);
            break;

        case ALARM_INTRUSION:{
            std::vector<DetectionLoop*> faultyLoops = _alarmManager.getTriggeredLoops();
            String names = "";
            //Listing opened loops names
            for(DetectionLoop *loop : faultyLoops){
                names += loop->getName() + ", ";
            }
            _communicationService.sendData(_config.keys.status, "INTRUSION EN COURS : " + names);
            break;
        }

        case ALARM_MANUAL_ON:
            //Informs the communication service that the manual mode was turned on
            _communicationService.sendData(_config.keys.manualMode, _config.values.manualModeOn);
            break;

        case ALARM_MANUAL_OFF:
            _communicationService.sendData(_config.keys.manualMode, _config.values.manualModeOff);

            break;

            
        default:
            break;
        }

}