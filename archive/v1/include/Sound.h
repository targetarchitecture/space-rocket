#ifndef Sound_h

#define Sound_h

#include <Arduino.h>
#include <list>
#include <Wire.h>
#include "DFRobotDFPlayerMini.h"

extern void sendMessage(String);

class Sound
{
public:
    void begin();
    void loop();
    String execute( std::list<String> values);

private:
    DFRobotDFPlayerMini myDFPlayer;
    int busyPin = 25;
    bool busy();
    bool previousBusy = 0;
};

#endif
