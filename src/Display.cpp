#include <Arduino.h>
#include "Display.h"

void Display::begin()
{
    tft.init();

    tft.setRotation(1);

    tft.fillScreen(backgroundColour);

    // Set "cursor" at top left corner of display (0,0) and select font 2
    tft.setCursor(0, 0, 1);

    // Set the font colour to be white with a black background
    tft.setTextColor(TFT_GREEN, backgroundColour);

    // We can now plot text on screen using the "print" class
    tft.println("42");
}

void Display::loop()
{
    if (useDisplay == true)
    {
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            show();
        }
    }
}

void Display::execute(String topic, String payload)
{
    if (topic.equalsIgnoreCase("sn1/display/background/colour"))
    {
        int rgb = payload.toInt();

        backgroundColour = rgb;

        String msg = "Display colour changed to ";
        msg.concat(backgroundColour);

        state.current(msg);
    }
}

void Display::printMessage(String line)
{
    //Serial.println(line);

    lines.push_back(line);

    if (lines.size() > 6)
    {
        lines.erase(lines.begin());
    }

    show();
}

void Display::show()
{
    if (useDisplay == false)
    {
        return;
    }

    //tft.fillScreen(backgroundColour);

    tft.setTextSize(1);  // Font size
    tft.setCursor(0, 0); // Start at top-left corner
    tft.setTextColor(TFT_SKYBLUE, backgroundColour);
    tft.println(F("Ada's Super Computer"));
    tft.setTextColor(0xFC9F, backgroundColour);
    tft.setTextSize(2);
    tft.print(F("Time: "));
    tft.println(millis());

    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW, backgroundColour);

    for (unsigned i = 0; i < lines.size(); ++i)
    {
        String line = lines[i];
        String padding = "";

        for (unsigned x = line.length(); x < textWidth; ++x)
        {
            padding += " ";
        }

        line += padding;

        tft.println(line);
    }
}
