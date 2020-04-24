#include <Arduino.h>
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

    // printMessage("Breath LED on pin 15");
    // sx1509.pinMode(15, ANALOG_OUTPUT);          // To breathe an LED, make sure you set it as an ANALOG_OUTPUT, so we can PWM the pin
    // sx1509.breathe(15, 1000, 1000, 1000, 1000); // Breathe an LED: 1000ms LOW, 500ms HIGH, 500ms to rise from low to high, 250ms to fall from high to low

    // printMessage("Blink LED on pin 0");
    // sx1509.pinMode(0, OUTPUT);  // Set LED pin to OUTPUT
    // sx1509.blink(0, 1000, 500); // Blink the LED pin -- ~1000 ms LOW, ~500 ms HIGH:

    // printMessage("ON LED on pin 8");
    // sx1509.pinMode(8, OUTPUT);    // Set LED pin to OUTPU
    // sx1509.digitalWrite(8, HIGH); //set to ON for Ada!

    // printMessage("Breath LED on pin 7");
    // sx1509.pinMode(7, ANALOG_OUTPUT);
    // sx1509.breathe(7, 1000, 1000, 1000, 1000); // Breathe an LED: 1000ms LOW, 500ms HIGH, 500ms to rise from low to high, 250ms to fall from high to low
}