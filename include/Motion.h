#ifndef Motion_h
#define Motion_h

#include <Arduino.h>
#include <State.h>
#include <list>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

extern void sendMessage(String topic, String message);
extern void sendMessage(String topic, uint8_t message);
extern void printMessage(String message);
extern void printMessage(String message, uint8_t arg1);
extern State state;

class Motion
{
public:
  void begin();
  void loop();
  void execute(String topic, String payload);
  bool motionWorking = false;

private:
  Adafruit_PWMServoDriver PCA9685 = Adafruit_PWMServoDriver(); // called this way, it uses the default address 0x40

  std::list<String> split(String msg);
  bool enabled[16];
};

#endif
