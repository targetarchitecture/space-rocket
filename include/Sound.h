#ifndef Sound_h

#define Sound_h

#include <Arduino.h>
#include <list>
#include <string>
#include <Wire.h>
#include "DFRobotDFPlayerMini.h"

extern void sendMessage(String topic, String message);
extern void sendMessage(String topic, uint8_t message);
extern void printMessage(String message);
extern void printMessage(String message, uint8_t arg1);

class Sound
{
public:
    void begin();
    void loop();
    void execute(String topic, String payload);
    void detail(uint8_t type, int value);

private:
    DFRobotDFPlayerMini myDFPlayer;
    const uint8_t busyPin = 25;
    int busy();
    int previousBusy = -1;
    int fileCounts;
    const uint32_t waitPeriod = 3000;
    void playTrack(uint8_t track);
};

#endif
