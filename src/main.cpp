/*********************************************************
Ada's Super Computer
**********************************************************/
#include <Arduino.h>

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

void handleEvents(String topic, String payload);
void printMessage(String message, uint8_t arg1);
void MQTT_begin();
void MQTT_loop();

const bool useDisplay = true;
const bool useDAC = false;
const bool useEncoders = false;

void setup()
{
    Wire.begin();         //I2C bus
    Serial.begin(115200); //ESP32 USB Port

    delay(100);

     if (useDisplay == true)
     {
        //SPI bus for display (faster frame rate)
        display.begin();
    }

    if (useDAC == true || useEncoders == true)
    {
        features.begin();
    }

    touch.begin();
    light.begin();
    motion.begin();
    sound.begin();

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

    delay(10);
}

void printMessage(String message)
{
    display.printMessage(message);
}

void printMessage(String message, uint8_t arg1)
{
    char buffer[50];
    sprintf(buffer, message.c_str(), arg1);

    display.printMessage(buffer);
}

void handleEvents(String topic, String payload)
{
    topic.toLowerCase();

    if (topic.startsWith("sn1/sound/") == true)
    {
        sound.execute(topic, payload);
    }

    if (topic.startsWith("sn1/light/") == true)
    {
        light.execute(topic, payload);
    }

    if (topic.startsWith("sn1/motion/") == true)
    {
        motion.execute(topic, payload);
    }
}
