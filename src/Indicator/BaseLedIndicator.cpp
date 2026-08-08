#include <BaseLedIndicator.h>

BaseLedIndicator::BaseLedIndicator(int pinLed):
_led(pinLed){

}

void BaseLedIndicator::init(){
    _led.init();
}

void BaseLedIndicator::update(){
    _led.update();
}
