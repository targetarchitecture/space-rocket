#ifndef Motion_h

#define Motion_h

#include <Arduino.h>
#include <list>
#include <Wire.h>
#include "ServoEasing.h"

extern void sendMessage(String);

class Motion
{
public:
  void begin();
  void loop();
  void execute(std::list<String> values);

private:
  void attachServos();
  void attachResults(int pin, uint8_t results);
  ServoEasing Servos[16];
  bool previouscurrentInterruptsActive = 0;
};

#endif
