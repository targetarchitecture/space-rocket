#include <Arduino.h>
#include "Motion.h"

void Motion::begin()
{
  PCA9685.begin();
  PCA9685.setOscillatorFrequency(27000000); // The int.osc. is closer to 27MHz
  PCA9685.setPWMFreq(50);                   // Analog servos run at ~50 Hz updates

  Wire.beginTransmission(0x40);
  uint8_t tWireReturnCode = Wire.endTransmission(true);
  if (tWireReturnCode == 0)
  {
    printMessage("Found motion controller");

    motionWorking = true;
  }
  else
  {
    printMessage("Motion controller not found");
    printMessage("Error code=%i", tWireReturnCode);

    motionWorking = false;
  }
}

void Motion::execute(String topic, String payload)
{
  if (motionWorking == false)
  {
    return;
  }

  try
  {
    if (topic.equalsIgnoreCase("sn1/motion/pwm"))
    {
      std::list<String> values = split(payload);

      long pin = values.front().toInt();
      values.pop_front();
      long microseconds = values.front().toInt();

      PCA9685.writeMicroseconds(pin, microseconds); 

      delay(10);
    }

  }
  catch (int e)
  {
    Serial.print("An exception occurred. Exception Nr. ");
    Serial.println(e, DEC);
  }
}

void Motion::loop()
{
  if (motionWorking == true)
  {
  }
}

//http://www.cplusplus.com/reference/list/list/pop_front/
std::list<String> Motion::split(String msg)
{
  std::list<String> subStrings;
  int j = 0;

  for (int i = 0; i < msg.length(); i++)
  {
    if (msg.charAt(i) == ',')
    {
      subStrings.push_back(msg.substring(j, i));
      j = i + 1;
    }
  }
  subStrings.push_back(msg.substring(j, msg.length())); //to grab the last value of the string
  return subStrings;
}