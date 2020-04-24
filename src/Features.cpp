#include <Arduino.h>
#include "Features.h"



void Features::begin()
{
    //set roatary encoder pins
    pinMode(35, INPUT);
    pinMode(39, INPUT);

    // Enable the weak pull down resistors
    ESP32Encoder::useInternalWeakPullResistors = true;

    // set starting count value
    encoder1.setCount(0);
    encoder2.setCount(0);

    // Attach pins for use as encoder pins
    encoder1.attachHalfQuad(36, 39);

    // Attach pins for use as encoder pins
    encoder2.attachHalfQuad(34, 35);

    printMessage("Setup Encoders");

    String response = "";

    response.concat("Setup DAC (");
    response.concat(DAC2);
    response.concat(")");

    printMessage(response);
}

void Features::loop()
{
    int32_t newEncoder1Count = encoder1.getCount();
    int32_t newEncoder2Count = encoder2.getCount();

    if (newEncoder1Count != encoder1Count)
    {
        String response = "";

        response.concat("{feature,encoder,1,");
        response.concat(newEncoder1Count);
        response.concat("}");

        sendDataToMicrobit(response);

        encoder1Count = newEncoder1Count;
    }

    if (newEncoder2Count != encoder2Count)
    {
        String response = "";

        response.concat("{feature,encoder,2,");
        response.concat(newEncoder2Count);
        response.concat("}");

        sendDataToMicrobit(response);

        encoder2Count = newEncoder2Count;
    }
}

void Features::setDACvoltage(double volts)
{
    uint8_t value = (255 / 3.3) * volts;

    dacWrite(DAC2, value);

    String response = "";

    response.concat("Set DAC to ");
    response.concat(volts);
    response.concat("v");

    printMessage(response);
}

void Features::execute(std::list<String> values)
{
    String function = values.front();
    values.pop_front();

    Serial.print("function:");
    Serial.println(function);

    if (function.equalsIgnoreCase("setDAC"))
    {
        double volts = values.front().toDouble();

        Serial.print("setDAC:");
        Serial.println(volts);

        setDACvoltage(volts);
    }

    if (function.equalsIgnoreCase("encoderClear"))
    {
        long encoder = values.front().toInt();

        if (encoder == 1)
        {
            encoder1.clearCount();
        }
        else
        {
            encoder2.clearCount();
        }

        Serial.print("encoderClear:");
        Serial.println(encoder);
    }

    if (function.equalsIgnoreCase("encoderCount"))
    {
        long encoder = values.front().toInt();

        if (encoder == 1)
        {
            int32_t newEncoder1Count = encoder1.getCount();

            String response = "";

            response.concat("{feature,encoderCount,1,");
            response.concat(newEncoder1Count);
            response.concat("}");

            sendDataToMicrobit(response);
        }
        else
        {
            int32_t newEncoder2Count = encoder2.getCount();

            String response = "";

            response.concat("{feature,encoderCount,2,");
            response.concat(newEncoder2Count);
            response.concat("}");

            sendDataToMicrobit(response);
        }
    }
}