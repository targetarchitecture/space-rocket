#include <Arduino.h>
#include "Motion.h"

void Motion::begin()
{
  PCA9685.begin();
  PCA9685.setOscillatorFrequency(27000000); // The int.osc. is closer to 27MHz
  PCA9685.setPWMFreq(SERVO_FREQ);           // Analog servos run at ~50 Hz updates

  // Wire.beginTransmission(PCA9685_DEFAULT_ADDRESS);
  // uint8_t tWireReturnCode = Wire.endTransmission(true);
  // if (tWireReturnCode == 0)
  // {
  //   printMessage("Found motion controller");
  // }
  // else
  // {
  //   printMessage("Motion controller not found");

  //   String response = "";

  //   response.concat("Error code=");
  //   response.concat(tWireReturnCode);

  //   printMessage(response);

  //   delay(500);
  //   ESP.restart();
  // }

  // if (tWireReturnCode == 0)
  // {
  //   attachServos();
  // }

  // for (int i = 0; i < 4097; i++)
  // {
  //   Serial.print("PWM:");
  //   Serial.println(i);

  //   Servos[15].setPWM(i);

  //   delay(100);
  // }
}

//https://learn.adafruit.com/making-adabot-part-2/source-code
float easeInOut( float t, float b, float c, float d )
{
  // Function used to smooth servo movements.
  if ((t/=d/2) < 1) 
    return c/2*t*t*t + b;
 
  return c/2*((t-=2)*t*t + 2) + b;
}

// void Motion::attachServos()
// {
//   for (int i = 0; i < 16; i++)
//   {
//     // Serial.print("attaching servo:");
//     // Serial.println(i);

//     attachResults(i, Servos[i].attach(i));
//   }

//   //Serial.println("attached servos");
// }

// void Motion::attachResults(int pin, uint8_t results)
// {
//   if (results == INVALID_SERVO)
//   {
//     //Serial.println(F("Error attaching servo - maybe MAX_EASING_SERVOS=" STR(MAX_EASING_SERVOS) " is to small to hold all servos"));
//     while (true)
//     {
//       digitalWrite(LED_BUILTIN, HIGH);
//       delay(100);
//       digitalWrite(LED_BUILTIN, LOW);
//       delay(100);
//     }
//   }
// }

void Motion::execute(String topic, String payload)
{
  try
  {
    // String function = values.front();
    // values.pop_front();
    // int servo = values.front().toInt();
    // values.pop_front();

    // Serial.print("function:");
    // Serial.println(function);
    // Serial.print("servo:");
    // Serial.println(servo);

    String function = payload;

    if (function.equalsIgnoreCase("startPosition"))
    {
      // int startPosition = values.front().toInt();

      // Serial.print("startPosition:");
      // Serial.println(startPosition);

      // Servos[servo].write(startPosition);
    }

    if (function.equalsIgnoreCase("stop"))
    {
      // int currentAngle = Servos[servo].getCurrentAngle();

      //  Servos[servo].write(currentAngle);
    }

    if (function.equalsIgnoreCase("moveTo"))
    {
      // int degree = values.front().toInt();
      // values.pop_front();
      // int speed = values.front().toInt();

      // Serial.print("degree:");
      // Serial.println(degree);
      // Serial.print("speed:");
      // Serial.println(speed);

      // Servos[servo].startEaseTo(degree, speed);
    }

    if (function.equalsIgnoreCase("setEasingType"))
    {
   //   uint8_t easing = values.front().toInt();

      //   Servos[servo].setEasingType(easing);
    }

    if (function.equalsIgnoreCase("setPWM"))
    {
     // uint8_t pwm = values.front().toInt();

      //    Servos[servo].setPWM(pwm);
    }

    if (function.equalsIgnoreCase("stopAll"))
    {
      //  stopAllServos();
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
  //unsigned long currentMillis = millis();

  // if (currentMillis - previousMillis > 20) {

  //   previousMillis = currentMillis;

  //   PCA9685.writeMicroseconds(1, map(degree, 0, 180, USMIN, USMAX));

  //   degree = degree + delta;

  //   if ((degree <= 0) || (degree >= 180)) {
  //     delta = delta * -1;
  //   }
}

// bool currentInterruptsActive = isOneServoMoving();

// if (currentInterruptsActive != previouscurrentInterruptsActive)
// {
//   String response = "";

//   response.concat("{motion,moving,");
//   response.concat(currentInterruptsActive);
//   response.concat("}");

//   sendMessage(response);

//   previouscurrentInterruptsActive = currentInterruptsActive;
// }
//}
