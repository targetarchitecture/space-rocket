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
  }
  else
  {
    printMessage("Motion controller not found");

    // String response = "";
    // response.concat("Error code=");
    // response.concat(tWireReturnCode);

    printMessage("Error code=%i",tWireReturnCode);

    // delay(500);
    // ESP.restart();
  }
}

void Motion::execute(String topic, String payload)
{
  try
  {
    if (topic.equalsIgnoreCase("sn1/motion/pwm"))
    {
      //Serial.print("servo PWM payload:");
      //Serial.println(payload);

      std::list<String> values = split(payload);

      //values.pop_front();
      long pin = values.front().toInt();
      values.pop_front();
      long microseconds = values.front().toInt();

      //Serial.print("servo pin:");
      //Serial.println(pin);
      //Serial.print("servo ms:");
      //Serial.println(microseconds);

      PCA9685.writeMicroseconds(pin, microseconds); //map(degree, 0, 180, 380, 2540));

      delay(10);      
    }

    // if (topic.equalsIgnoreCase("stopAll"))
    // {
    //   //  stopAllServos();
    // }
  }
  catch (int e)
  {
    Serial.print("An exception occurred. Exception Nr. ");
    Serial.println(e, DEC);
  }
}

void Motion::loop()
{
  //unsigned long currentMillis = millis();

  // if (currentMillis - previousMillis > 20) {

  //   previousMillis = currentMillis;

  // PCA9685.w..writeMicroseconds(1, map(degree, 0, 180, USMIN, USMAX));

  //   degree = degree + delta;

  //   if ((degree <= 0) || (degree >= 180)) {
  //     delta = delta * -1;
  //   }
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