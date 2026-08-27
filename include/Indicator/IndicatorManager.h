#ifndef INDICATOR_MANAGER_H
#define INDICATOR_MANAGER_H

#include <Arduino.h>
#include <devices/DetectionLoop.h>
#include <AlarmManager.h>
#include <LoopIndicator.h>
#include <AlarmIndicator.h>
#include <vector>
/**
 * @class IndicatorManager
 * @brief Controls the LED indicators based on the status of the loops and the alarm system.
 */
class IndicatorManager{

private:
    AlarmManager& _alarm;

    //Led indicators
    AlarmIndicator* _alarmIndicator = nullptr;
    std::vector<LoopIndicator*> _loopIndicators;
    
public:
    /**
     * @brief Constructor
     * @param alarmManager The instance of the Alarm Manager
     */
    IndicatorManager(AlarmManager &alarmManager);

    /**
     * @brief Updates each loop indicator and the alarm indicator
     */
    void update();

    /**
     * @brief Adds the indicator to the list and
     * automatically subscribes it to the desired loop
     */
    void addLoopIndicator(LoopIndicator &loopIndicator, DetectionLoop &subjectLoop);

    /**
     * @brief Adds the indicator and
     * automatically subscribes it to the alarm manager
     */
    void setAlarmIndicator(AlarmIndicator &alarmIndicator);


};

#endif