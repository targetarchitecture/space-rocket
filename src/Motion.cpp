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

    state.current("Found motion controller");

    motionWorking = true;

    //initialise enabled array to allow servos to run
    for (size_t i = 0; i < 16; i++)
    {
      enabled[i] = true;
    }
  }
  else
  {
    printMessage("Motion controller not found");
    printMessage("Error code=%i", tWireReturnCode);

    state.warning("Motion controller not found");

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

    if (topic.equalsIgnoreCase("sn1/motion/stop"))
    {
      long pin = payload.toInt();

      PCA9685.setPin(pin,0);

      delay(10);

      enabled[pin] = false;
    }

    if (topic.equalsIgnoreCase("sn1/motion/start"))
    {
      long pin = payload.toInt();

      enabled[pin] = true;
    }

    if (topic.equalsIgnoreCase("sn1/motion/us"))
    {
      std::list<String> values = split(payload);

      long pin = values.front().toInt();
      values.pop_front();
      long microseconds = values.front().toInt();

      if (enabled[pin] == true)
      {
        PCA9685.writeMicroseconds(pin, microseconds);
      }
      else
      {
        state.warning("Tried to move servo that is disabled");
      }

      delay(10);
    }

    if (topic.equalsIgnoreCase("sn1/motion/pwm"))
    {
      std::list<String> values = split(payload);

      long pin = values.front().toInt();
      values.pop_front();
      long pulse = values.front().toInt();

      if (enabled[pin] == true)
      {
        PCA9685.setPWM(pin, 0, pulse);
      }
      else
      {
        state.warning("Tried to move servo that is disabled");
      }

      delay(10);
    }
  }
  catch (int e)
  {
    state.error("An exception occured in motion execute");

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