#include <Arduino.h>
#include <limits.h>
#include "Light.h"

void Light::begin()
{
    // Call io.begin(<address>) to initialize the SX1509. If it successfully communicates, it'll return 1.
    if (!sx1509.begin(SX1509_ADDRESS))
    {
        printMessage("SX1509 not found");
        delay(500);
        ESP.restart();
    }
    else
    {
        printMessage("SX1509 lighting found");

        // Use the internal 2MHz oscillator.
        // Set LED clock to 500kHz (2MHz / (2^(3-1)):
        sx1509.clock(INTERNAL_CLOCK_2MHZ, 3);
    }
}

void Light::execute(std::list<String> values)
{
    try
    {
        String function = values.front();
        values.pop_front();
        int pin = values.front().toInt();

        Serial.print("function:");
        Serial.println(function);
        Serial.print("pin:");
        Serial.println(pin);

        if (function.equalsIgnoreCase("on"))
        {
            for (const String &otherPins : values)
            {
                //set method for the pins so we can figure out how to turn it
                Method[otherPins.toInt()] = "on";

                sx1509.pinMode(otherPins.toInt(), OUTPUT);    // Set LED pin to OUTPUT
                sx1509.digitalWrite(otherPins.toInt(), HIGH); //set to ON for Ada!

                delay(10);
            }
        }

        if (function.equalsIgnoreCase("off"))
        {
            for (const String &otherPins : values)
            {
                if (Method[pin].equalsIgnoreCase("on"))
                {
                    sx1509.digitalWrite(otherPins.toInt(), LOW); //set to OFF
                }
                if (Method[pin].equalsIgnoreCase("blink"))
                {
                    sx1509.setupBlink(otherPins.toInt(), 0, 0);
                }
                if (Method[pin].equalsIgnoreCase("breathe"))
                {
                    sx1509.setupBlink(otherPins.toInt(), 0, 0);
                }
                delay(10);
            }
        }

        if (function.equalsIgnoreCase("breathe"))
        {
            values.pop_front();
            int timeOn = values.front().toInt();
            values.pop_front();
            int timeOff = values.front().toInt();
            values.pop_front();
            int timeRise = values.front().toInt();
            values.pop_front();
            int timeFall = values.front().toInt();

            sx1509.pinMode(pin, ANALOG_OUTPUT);                       // To breathe an LED, make sure you set it as an ANALOG_OUTPUT, so we can PWM the pin
            sx1509.breathe(pin, timeOn, timeOff, timeRise, timeFall); // Breathe an LED: 1000ms LOW, 500ms HIGH, 500ms to rise from low to high, 250ms to fall from high to low

            //set method for the pins so we can figure out how to turn it
            Method[pin] = "breathe";

            Serial.print("timeOn:");
            Serial.println(timeOn);
            Serial.print("timeOff:");
            Serial.println(timeOff);
            Serial.print("timeRise:");
            Serial.println(timeRise);
            Serial.print("timeFall:");
            Serial.println(timeFall);

            //sx1509.breathe(pin, 1000, 5000, 3000, 2500);
        }

        if (function.equalsIgnoreCase("blink"))
        {
            values.pop_front();
            int timeOn = values.front().toInt();
            values.pop_front();
            int timeOff = values.front().toInt();

            sx1509.pinMode(pin, ANALOG_OUTPUT); // Set LED pin to OUTPUT
            sx1509.blink(pin, timeOn, timeOff); // Blink the LED pin -- ~1000 ms LOW, ~500 ms HIGH:

            //set method for the pins so we can figure out how to turn it
            Method[pin] = "blink";
        }
    }
    catch (int e)
    {
        Serial.print("An exception occurred. Exception Nr. ");
        Serial.println(e, DEC);
    }
}