#ifndef Light_h

#define Light_h

#include <Arduino.h>
#include <Wire.h>
#include <SparkFunSX1509.h> // Include SX1509 library

extern void printMessage(String);

class Light
{
public:
  void begin();
  void loop();

private:
  // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
  const byte SX1509_ADDRESS = 0x3E; // SX1509 I2C address
  SX1509 sx1509;                    // Create an SX1509 object to be used throughout
};

#endif
