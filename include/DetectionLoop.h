#ifndef DETECTION_LOOP_H
#define DETECTION_LOOP_H
#include <Arduino.h>
class DetectionLoop
{
private:
    String name;
    bool isOpen;
public:
    DetectionLoop(int pin, String name);

    String getName();
    void setName(String name);
    bool isOpen();


};

#endif
