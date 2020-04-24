#include <Arduino.h>
#include "Motion.h"

void Motion::begin()
{
  // Initialize wire before checkI2CConnection()
  Serial.println(F("Try to communicate with PCA9685 Expander by TWI / I2C"));
  Serial.flush();
  Wire.beginTransmission(PCA9685_DEFAULT_ADDRESS);
  uint8_t tWireReturnCode = Wire.endTransmission(true);
  if (tWireReturnCode == 0)
  {
    Serial.print(F("Found"));
  }
  else
  {
    Serial.print(F("Error code="));
    Serial.print(tWireReturnCode);
    Serial.print(F(". Communication with I2C was successful, but found no"));
  }
  Serial.print(F(" I2C device attached at address: 0x"));
  Serial.println(PCA9685_DEFAULT_ADDRESS, HEX);

  if (tWireReturnCode == 0)
  {
  attachServos();
  }

  // for (int i = 0; i < 4097; i++)
  // {
  //   Serial.print("PWM:");
  //   Serial.println(i);

  //   Servos[15].setPWM(i);

  //   delay(100);
  // }
}

void Motion::attachServos()
{
  for (int i = 0; i < 16; i++)
  {
    Serial.print("attaching servo:");
    Serial.println(i);

    attachResults(i, Servos[i].attach(i));
  }

  Serial.println("attached servos");
}

void Motion::attachResults(int pin, uint8_t results)
{
  if (results == INVALID_SERVO)
  {
    Serial.println(F("Error attaching servo - maybe MAX_EASING_SERVOS=" STR(MAX_EASING_SERVOS) " is to small to hold all servos"));
    while (true)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
}

void Motion::execute(std::list<String> values)
{
  String function = values.front();
  values.pop_front();
  int servo = values.front().toInt();
  values.pop_front();

  Serial.print("function:");
  Serial.println(function);
  Serial.print("servo:");
  Serial.println(servo);

  if (function.equalsIgnoreCase("startPosition"))
  {
    int startPosition = values.front().toInt();

    Serial.print("startPosition:");
    Serial.println(startPosition);

    Servos[servo].write(startPosition);
  }

  if (function.equalsIgnoreCase("stop"))
  {
    int currentAngle = Servos[servo].getCurrentAngle();

    Servos[servo].write(currentAngle);
  }

  if (function.equalsIgnoreCase("moveTo"))
  {
    int degree = values.front().toInt();
    values.pop_front();
    int speed = values.front().toInt();

    Serial.print("degree:");
    Serial.println(degree);
    Serial.print("speed:");
    Serial.println(speed);

    Servos[servo].startEaseTo(degree, speed);
  }

  if (function.equalsIgnoreCase("setEasingType"))
  {
    uint8_t easing = values.front().toInt();

    Servos[servo].setEasingType(easing);
  }

  if (function.equalsIgnoreCase("setPWM"))
  {
    uint8_t pwm = values.front().toInt();

    Servos[servo].setPWM(pwm);
  }

  if (function.equalsIgnoreCase("stopAll"))
  {
    stopAllServos();
  }
}

void Motion::loop()
{
  bool currentInterruptsActive = isOneServoMoving();

  if (currentInterruptsActive != previouscurrentInterruptsActive)
  {
    String response = "";

    response.concat("{motion,moving,");
    response.concat(currentInterruptsActive);
    response.concat("}");

    sendMessage(response);

    previouscurrentInterruptsActive = currentInterruptsActive;
  }
}
