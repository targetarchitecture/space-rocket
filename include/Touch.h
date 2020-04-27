#ifndef Touch_h

#define Touch_h

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

//extern void sendMessage(String);
//extern void sendMessage(char *topic, char *message);
template <typename... Args>
extern void sendMessage(const char *topic,const char *message, const Args & ... args);

//extern void sendMessage(const char *topic, const char *message, fmt::format_args args);

class Touch
{
public:
    void begin();
    void loop();

private:
    Adafruit_MPR121 cap = Adafruit_MPR121();
    // Keeps track of the last pins touched
    // so we know when buttons are 'released'
    uint16_t lasttouched = 0;
    uint16_t currtouched = 0;
};

#endif
