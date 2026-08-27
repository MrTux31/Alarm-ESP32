#include <AlarmIndicator.h>

AlarmIndicator::AlarmIndicator(int pinLed): BaseLedIndicator(pinLed) {

}

void AlarmIndicator::update(AlarmManager *subject, AlarmManagerEvent event){
    switch (event){

        case ALARM_ARMING:
            _led.blink(ARMING_BLINKING_DELAY);
            break;

        case ALARM_ARMED:
            _led.turnOn();
            break;

        case ALARM_INTRUSION:
            //Ultra fast blinking
            _led.blink(INTRUSION_BLINKING_DELAY); 
            break;

        case ALARM_STANDBY:
            _led.turnOn();
            break;

        case ALARM_DISARMED:
            _led.turnOff();
            break;

    }
}