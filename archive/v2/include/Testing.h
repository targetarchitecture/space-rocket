#ifndef Testing_h

#define Testing_h

#include <Arduino.h>
#include <vector>

extern void sendMessage(String);
extern void printMessage(String);
extern String message;

class Testing
{
public:
  void setup();
  void loop();

private:
  std::vector<String> tests;
  unsigned long previousMillis = 0;
  unsigned int currentTest = 0;
  unsigned int numberOfTests = 0;
  const long interval = 3000;
   unsigned long sweepServoPreviousMillis = 0;
  void SweepServo();
};

#endif
