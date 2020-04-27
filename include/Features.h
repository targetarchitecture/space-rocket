#ifndef Features_h
#define Features_h

#include <Arduino.h>
#include <Wire.h>
#include <ESP32Encoder.h>

extern void sendMessage(String topic, String message);
extern void sendMessage(String topic, uint8_t message);
extern void printMessage(String message);
extern void printMessage(String message, uint8_t arg1);

class Features
{
public:
  void begin();
  void loop();
  void execute(String topic, String payload);

private:
  void setDACvoltage(double volts);
  ESP32Encoder encoder1;
  ESP32Encoder encoder2;
  int32_t encoder1Count = 0;
  int32_t encoder2Count = 0;
};

#endif
