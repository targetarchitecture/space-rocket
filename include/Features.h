#ifndef Features_h

#define Features_h

#include <Arduino.h>
#include <list>
#include <Wire.h>
//#include <fmt/format.h>
#include <ESP32Encoder.h>

template <typename... Args>
extern void sendMessage(const char *topic,const char *message, const Args & ... args);

//extern void sendMessage(const char *topic, const char *message, fmt::format_args args);
//extern void sendMessage(char *topic, char *message);
//extern void sendMessage(String);
extern void printMessage(String);

class Features
{
public:
  void begin();
  void loop();
  //void execute(std::list<String> values);
  void execute(String topic, String payload);

private:
  void setDACvoltage(double volts);
  ESP32Encoder encoder1;
  ESP32Encoder encoder2;
  int32_t encoder1Count = 0;
  int32_t encoder2Count = 0;
};

#endif
