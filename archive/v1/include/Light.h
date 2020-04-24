#ifndef Light_h

#define Light_h

#include <Arduino.h>
#include <Wire.h>
#include <SparkFunSX1509.h> // Include SX1509 library

class Light
{
public:
  //Light();
  void begin();
  void loop();

private:
  // Adafruit_VL53L0X lox;
};

#endif
