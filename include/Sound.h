#ifndef Sound_h

#define Sound_h

#include <Arduino.h>
#include <list>
#include <Wire.h>
//#include <fmt/core.h>
//#include <fmt/format.h>
#include "DFRobotDFPlayerMini.h"

//extern void sendMessage(String);
//extern void sendMessage(char *topic, char *message);
template <typename... Args>
extern void sendMessage(const char *topic,const char *message, const Args & ... args);
extern void printMessage(String);

class Sound
{
public:
    void begin();
    void loop();
    void execute(String topic, String payload);

private:
    DFRobotDFPlayerMini myDFPlayer;
    const uint8_t busyPin = 25;
    int busy();
    int previousBusy = -1;
    int fileCounts;
    const uint32_t waitPeriod = 100;
};

#endif
