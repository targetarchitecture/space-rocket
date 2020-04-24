/*********************************************************
Ada's Super Computer
**********************************************************/
#include <Arduino.h>
#include <list>

#include "Display.h"
#include "Sound.h"
#include "Touch.h"
#include "Light.h"
#include "Motion.h"
#include "Features.h"
#include "Testing.h"
#include "BLE_Serial.h"

Display display;
Sound sound;
Motion motion;
Touch touch;
Features features;
Light light;
Testing testing;

String message = ""; // a string to hold incoming data

void i2cScan();
void printMessage(String);
void handleEvents();
std::list<String> splitStringToList(String);

void setup()
{
  Serial.begin(115200);  //ESP32 USB Port
  Serial2.begin(115200); //BBC Microbit Serial
  Wire.begin();          //I2C bus

  delay(10);

  display.begin();

  i2cScan();

  sound.begin();
  touch.begin();
  features.begin();
  light.begin();
  motion.begin();
  BLESerial_begin();

  //clear the buffers to begin processing
  delay(10);
  Serial.flush();
  Serial2.flush();
  delay(10);

  //start tests
  //testing.setup();
}

void loop()
{
  // testing.loop();
  display.loop();
  sound.loop();
  motion.loop();
  touch.loop();
  features.loop();

  BLESerial_loop();

  handleEvents();

  delay(10);
}

void printMessage(String message)
{
  display.printMessage(message);
}

void handleEvents()
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

  std::list<String> splitMessage;

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

    if (action.equalsIgnoreCase("s"))
    {
      String result;
      result = sound.execute(values);

      if (result.isEmpty() == false)
      {
        sendMessage(result);
      }
    }
    else if (action.equalsIgnoreCase("m"))
    {
      motion.execute(values);
    }
    else if (action.equalsIgnoreCase("f"))
    {
      features.execute(values);
    }

    message = "";
  }
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
