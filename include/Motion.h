#ifndef Motion_h

#define Motion_h

#include <Arduino.h>
#include <list>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//extern void sendMessage(String);
extern void sendMessage(char *topic, char *message);
extern void printMessage(String);

class Motion
{
public:
  void begin();
  void loop();
  void execute(std::list<String> values);

private:
  //void attachServos();
  //void attachResults(int pin, uint8_t results);
  //ServoEasing Servos[16];
  //bool previouscurrentInterruptsActive = 0;

  // called this way, it uses the default address 0x40
  Adafruit_PWMServoDriver PCA9685 = Adafruit_PWMServoDriver();

  unsigned long previousMillis = 0;
  int degree = 90;
  int delta = 1;

  // Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
//#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
//#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
const uint USMIN = 380;     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
const uint USMAX =2540 ;   // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
const uint SERVO_FREQ =50 ;// Analog servos run at ~50 Hz updates
};

#endif
