#ifndef Light_h
#define Light_h

#include <Arduino.h>
#include <list>
#include <Wire.h>
#include <SparkFunSX1509.h> // Include SX1509 library

extern void sendMessage(String topic, String message);
extern void sendMessage(String topic, uint8_t message);
extern void printMessage(String message);
extern void printMessage(String message, uint8_t arg1);

class Light
{
public:
  void begin();
  void loop();
  void execute(String topic, String payload);

private:
  std::list<String> split(String msg);
  // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
  const byte SX1509_ADDRESS = 0x3E; // SX1509 I2C address
  SX1509 sx1509;                    // Create an SX1509 object to be used throughout
  String Method[16];
};

#endif
