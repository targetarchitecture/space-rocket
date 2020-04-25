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
//#include "MQTTWorker.h"

Display display;
Sound sound;
Motion motion;
Touch touch;
Features features;
Light light;

String message = ""; // a string to hold incoming data

void printMessage(String);
void handleEvents();
//void sendMessage(String);
std::list<String> splitStringToList(String);
//std::list<String> debugs(String);
void MQTT_begin();
void MQTT_loop();

void setup()
{
    Serial.begin(115200); //ESP32 USB Port
    Wire.begin();         //I2C bus

    delay(10);

    display.begin();
    sound.begin();
    touch.begin();
    features.begin();
    light.begin();
    motion.begin();

    MQTT_begin();
}

void loop()
{
    display.loop();
    sound.loop();
    motion.loop();
    touch.loop();
    features.loop();

    MQTT_loop();

    handleEvents();

    delay(10);
}

void printMessage(String message)
{
    display.printMessage(message);
}

void handleEvents()
{
    //   if (std::string(topic) == std::string(MQTT_STOP_TOPIC))
    //   {
    //     if (message.equalsIgnoreCase("stop") == true)
    //     {
    //       dfPlayer.stop(); // Stop the track
    //       delay(200);
    //       publishMQTTmessage("Stopped play");
    //     }
    //   }

    //   if (std::string(topic) == std::string(MQTT_PLAY_TOPIC))
    //   {
    //     int playTrack = message.toInt();

    //     dfPlayer.stop(); // Stop the track
    //     delay(200);

    //     dfPlayer.play(playTrack); // Play the track specified

    //     publishMQTTmessage("Playing track " + message);
    //   }

    //   if (std::string(topic) == std::string(MQTT_VOLUME_TOPIC))
    //   {
    //     int nvsVolume = message.toInt();

    //     dfPlayer.volume(nvsVolume);
    //     delay(200);

    //     publishMQTTmessage("Volume set to " + message);

    //     settings.putInt("volume", nvsVolume);
    //     nvsVolume = settings.getInt("volume", DefaultVolume);
    //     publishMQTTmessage("Volume from NVS: " + String(nvsVolume));
    //   }

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
