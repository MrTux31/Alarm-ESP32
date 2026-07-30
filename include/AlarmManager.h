
#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H
#include <Arduino.h>
#include <vector>
#include <DetectionLoop.h>

enum State { DISARMED, EXIT_DELAY, ARMED, ENTRY_DELAY, ALARM_ACTIVE };

class AlarmManager{

private:
    std::vector<DetectionLoop> loops;
public:
    AlarmManager(std::vector<DetectionLoop> loops); 
};



#endif