#ifndef Sound_h

#define Sound_h

#include <Arduino.h>
#include <list>
#include <Wire.h>
#include "DFRobotDFPlayerMini.h"

extern void sendDataToMicrobit(String);
extern void printMessage(String);

class Sound
{
public:
    void begin();
    void loop();
    void execute(std::list<String> values);

private:
    DFRobotDFPlayerMini myDFPlayer;
    const uint8_t busyPin = 25;
    int busy();
    int previousBusy = -1;
    int fileCounts;
    const uint32_t waitPeriod = 100;
};

#endif
