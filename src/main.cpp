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

Display display;
Sound sound;
Motion motion;
Touch touch;
Features features;
Light light;

String message = ""; // a string to hold incoming data

void printMessage(String);
void handleEvents();
void sendDataToMicrobit(String message);
void getDataFromMicrobit();
std::list<String> splitStringToList(String);
std::list<String> debugs(String);

void setup()
{
    Serial.begin(115200);  //ESP32 USB Port
    Serial2.begin(115200); //BBC Microbit Serial
    Wire.begin();          //I2C bus

    delay(10);

    display.begin();

    sound.begin();
    touch.begin();
    features.begin();
    light.begin();
    motion.begin();

    //clear the buffers to begin processing
    delay(10);
    Serial.flush();
    Serial2.flush();
    delay(10);
}

void loop()
{
    display.loop();
    sound.loop();
    motion.loop();
    touch.loop();
    features.loop();

    getDataFromMicrobit();

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

        if (action.equalsIgnoreCase("sound"))
        {
            sound.execute(values);
        }
        else if (action.equalsIgnoreCase("motion"))
        {
            motion.execute(values);
        }
        else if (action.equalsIgnoreCase("features"))
        {
            features.execute(values);
        }
        else
        {
            Serial.println("Message recieved in loop from MicroBit:" + message);
            // Serial.println(message);
        }
        // else if (action.equalsIgnoreCase("message"))
        // {
        //     display.printMessage(message);
        // }

        message = "";
    }
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

void sendDataToMicrobit(String message)
{
    Serial.println("Sending to MicroBit:" + message);
    //Serial.println(message);
    //   Serial.flush();

    try
    {
        Serial2.println(message);
        Serial2.flush();
    }
    catch (int e)
    {
        Serial.print("An exception occurred sending to MicroBit. Exception Nr. ");
        Serial.println(e, DEC);
    }
}

void getDataFromMicrobit()
{
    //String partialMessage = ""; // a string to hold incoming data

    //   while (Serial.available())
    //   {
    //     // get the new byte:
    //     char inChar = (char)Serial.read();

    //     //end of message character
    //     if (inChar == '}') // '\n')
    //     {
    //       message = partialMessage;

    //       // clear the string:
    //       partialMessage = "";
    //     }
    //     else
    //     {
    //       // add it to the inputString:
    //       partialMessage += inChar;
    //     }
    //   }

    while (Serial2.available())
    {
        message = Serial2.readString();
        message.trim();

        //     // get the new byte:
        //     char inChar = (char)Serial2.read();

        //     //end of message character
        //     if (inChar == '}') // '\n')
        //     {
        //       message = partialMessage;

        //       // clear the string:
        //       partialMessage = "";
        //     }
        //     else
        //     {
        //       // add it to the inputString:
        //       partialMessage += inChar;
        //     }
    }

    if (message != "")
    {
        Serial.println("Recieved from MicroBit:" + message);
        // Serial.println(message);
        // Serial.flush();
    }
}