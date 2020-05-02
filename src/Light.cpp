#include <Arduino.h>
#include <limits.h>
#include "Light.h"

void Light::begin()
{
    // Call io.begin(<address>) to initialize the SX1509. If it successfully communicates, it'll return 1.
    if (!sx1509.begin(SX1509_ADDRESS))
    {
        printMessage("SX1509 not found");

        lightWorking = false;
    }
    else
    {
        printMessage("SX1509 lighting found");

        // Use the internal 2MHz oscillator.
        // Set LED clock to 500kHz (2MHz / (2^(3-1)):
        sx1509.clock(INTERNAL_CLOCK_2MHZ, 4);

        lightWorking = true;
    }
}

void Light::execute(String topic, String payload)
{
    if (lightWorking == false)
    {
        return;
    }

    try
    {
        if (topic.equalsIgnoreCase("sn1/light/on"))
        {
            std::list<String> pins = split(payload);

            for (const String &pin : pins)
            {
                //stop the LED first
                stop(pin.toInt());

                //set method for the pins so we can figure out how to turn it
                Method[pin.toInt()] = "on";

                sx1509.pinMode(pin.toInt(), OUTPUT);    // Set LED pin to OUTPUT
                sx1509.digitalWrite(pin.toInt(), HIGH); //set to ON for Ada!

                delay(10);
            }
        }

        if (topic.equalsIgnoreCase("sn1/light/off"))
        {
            std::list<String> pins = split(payload);

            for (const String &pin : pins)
            {
                stop(pin.toInt());
            }
        }

        if (topic.equalsIgnoreCase("sn1/light/blink"))
        {
            std::list<String> pins = split(payload);

            //pins.pop_front();
            int timeOn = pins.front().toInt();
            pins.pop_front();
            int timeOff = pins.front().toInt();
            pins.pop_front();

            for (const String &pin : pins)
            {
                //stop the LED first
                stop(pin.toInt());

                sx1509.pinMode(pin.toInt(), ANALOG_OUTPUT); // Set LED pin to OUTPUT
                sx1509.blink(pin.toInt(), timeOn, timeOff); // Blink the LED pin -- ~1000 ms LOW, ~500 ms HIGH:

                delay(10);

                //set method for the pins so we can figure out how to turn it
                Method[pin.toInt()] = "blink";
            }
        }

        if (topic.equalsIgnoreCase("sn1/light/breathe"))
        {
            std::list<String> pins = split(payload);

            //pins.pop_front();
            long timeOn = pins.front().toInt();
            pins.pop_front();
            long timeOff = pins.front().toInt();
            pins.pop_front();
            long timeRise = pins.front().toInt();
            pins.pop_front();
            long timeFall = pins.front().toInt();
            pins.pop_front();

            for (const String &pin : pins)
            {
                //stop the LED first
                stop(pin.toInt());

                sx1509.pinMode(pin.toInt(), ANALOG_OUTPUT);                       // To breathe an LED, make sure you set it as an ANALOG_OUTPUT, so we can PWM the pin
                sx1509.breathe(pin.toInt(), timeOn, timeOff, timeRise, timeFall); // Breathe an LED: 1000ms LOW, 500ms HIGH, 500ms to rise from low to high, 250ms to fall from high to low

                delay(10);

                //set method for the pins so we can figure out how to turn it
                Method[pin.toInt()] = "breathe";
            }
        }
    }
    catch (int e)
    {
        Serial.print("An exception occurred. Exception Nr. ");
        Serial.println(e, DEC);
    }
}

void Light::stop(long pin)
{
    if (Method[pin].equalsIgnoreCase("on"))
    {
        sx1509.digitalWrite(pin, LOW); //set to OFF
    }
    if (Method[pin].equalsIgnoreCase("blink"))
    {
        sx1509.setupBlink(pin, 0, 0);
    }
    if (Method[pin].equalsIgnoreCase("breathe"))
    {
        sx1509.setupBlink(pin, 0, 0);
    }
    delay(10);

    //set method for the pins so we can figure out how to turn it
    Method[pin] = "off";
}

//http://www.cplusplus.com/reference/list/list/pop_front/
std::list<String> Light::split(String msg)
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