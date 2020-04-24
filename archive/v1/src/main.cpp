/*********************************************************
Ada's Super Computer
**********************************************************/
#include <Arduino.h>
#include <list>

#include "Sound.h"
#include "Touch.h"
#include "Light.h"
#include "Motion.h"
#include "Features.h"

Sound sound;
Motion motion;
Touch touch;
Features features;
// Light light;

String partialMessage = ""; // a string to hold incoming data
String message = "";        // a string to hold incoming data
std::list<String> splitMessage;

void i2cScan();
void handleSerialEvents();
void getMessage();
void sendMessage(String);
std::list<String> splitStringToList(String);

void setup()
{
  Serial.begin(115200);  //ESP32 USB Port
  Serial2.begin(115200); //BBC Microbit Serial

  pinMode(35, INPUT);

  delay(10);

  Wire.begin();

  i2cScan();

  sound.begin();
  touch.begin();
  features.begin();
  // light.begin();
  motion.begin();

  delay(10);
  Serial.flush();
  Serial2.flush();
  delay(10);

  // message = "{motion,startPosition,1,0}";
  // handleSerialEvents();
  // delay(1000);
  // motion.loop();
  // message = "{motion,moveTo,1,100,5}";
  // handleSerialEvents();
  // motion.loop();
}

/*
{motion,moveTo,1,3,5}
{motion,stop,1}
{motion,stopAll}
*/

void loop()
{
  //not stable unless DFPlayer powered up
  //sound.loop();

  touch.loop();
  motion.loop();
  features.loop();

  getMessage();

  handleSerialEvents();

  delay(10);
}

void handleSerialEvents()
{
  if (message != "")
  {
    //remove troublesome characters
    message.replace("{", "");
    message.replace("}", "");
    message.replace("\n", "");
    message.trim();

    //debug and print back
    // Serial.print("message:");
    // Serial.println(message);

    splitMessage.clear();
    splitMessage = splitStringToList(message);

    String action = splitMessage.front();
    splitMessage.pop_front();

    std::list<String> values = splitMessage;

    //debug and print back
    // Serial.print("action:");
    // Serial.println(action);
    // Serial.print("function:");
    // Serial.println(function);
    // Serial.print("values:");
    // Serial.println(values);

    if (action.equalsIgnoreCase("sound"))
    {
      String result;
      result = sound.execute(values);

      if (result.isEmpty() == false)
      {
        sendMessage(result);
      }
    }
    else if (action.equalsIgnoreCase("motion"))
    {
      motion.execute(values);
    }
    else if (action.equalsIgnoreCase("feature"))
    {
      features.execute(values);
    }

    message = "";
  }
}

void sendMessage(String message)
{
  //TODO: debate if it needs a line break!
  Serial.println(message);

  try
  {
    Serial2.println(message);
  }
  catch (int e)
  {
    Serial.print("An exception occurred. Exception Nr. ");
    Serial.println(e, DEC);
  }
}

void getMessage()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();

    //end of message character
    if (inChar == '}') // '\n')
    {
      message = partialMessage;

      // clear the string:
      partialMessage = "";
    }
    else
    {
      // add it to the inputString:
      partialMessage += inChar;
    }
  }

  // while (Serial2.available())
  // {
  //   // get the new byte:
  //   char inChar = (char)Serial2.read();

  //   //end of message character
  //   if (inChar == '}') // '\n')
  //   {
  //     message = partialMessage;

  //     // clear the string:
  //     partialMessage = "";
  //   }
  //   else
  //   {
  //     // add it to the inputString:
  //     partialMessage += inChar;
  //   }
  // }
}

void i2cScan()
{
  byte error, address;
  int nDevices;

  Serial.println("");
  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    // The i2c_scanner uses the return value of the Write.endTransmisstion to see if a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}

//http://www.cplusplus.com/reference/list/list/pop_front/
std::list<String> splitStringToList(String msg)
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