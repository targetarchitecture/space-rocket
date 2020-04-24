#include <Arduino.h>
#include "Display.h"

void Display::begin()
{
    tft.init();
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);

    // Set "cursor" at top left corner of display (0,0) and select font 2
    tft.setCursor(0, 0, 1);

    // Set the font colour to be white with a black background
    tft.setTextColor(TFT_GREEN, TFT_BLACK);

    // We can now plot text on screen using the "print" class
    tft.println("42");
    // tft.println("White text");

    // tft.setTextColor(TFT_RED, TFT_BLACK);
    // tft.println("Red text");

    // tft.setTextColor(TFT_GREEN, TFT_BLACK);
    // tft.println("Green text");

    // tft.setTextColor(TFT_BLUE, TFT_BLACK);
    // tft.println("Blue text");
}

void Display::loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        show();
    }
}

void Display::printMessage(String line)
{
    lines.push_back(line);
    //Serial.println(line);

    if (lines.size() > 6)
    {
        lines.erase(lines.begin());
    }

    show();
}

void Display::show()
{
    //tft.fillScreen(TFT_BLACK);

    tft.setTextSize(1);  // Font size
    tft.setCursor(0, 0); // Start at top-left corner
    tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    tft.println(F("Ada's Super Computer"));
    tft.setTextColor(0xFC9F, TFT_BLACK);
    tft.setTextSize(2);
    tft.print(F("Time: "));
    tft.println(millis());

    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);

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
