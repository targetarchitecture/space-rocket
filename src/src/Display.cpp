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
        int rIndex = payload.indexOf("r");
        int gIndex = payload.indexOf("g");
        int bIndex = payload.indexOf("b");

        int rcIndex = payload.indexOf(",", rIndex);
        int gcIndex = payload.indexOf(",", gIndex);
        int bcIndex = payload.indexOf(",", bIndex);

        String r = payload.substring(rIndex+3, rcIndex);
        String g = payload.substring(gIndex+3, gcIndex);
        String b = payload.substring(bIndex+3, bcIndex);

        backgroundColour = tft.color565(r.toInt(), g.toInt(), b.toInt());

        tft.fillScreen(backgroundColour);

        // String msg = "Display colour changed to R:";
        // msg.concat(r);
        // msg.concat(" , G:");
        // msg.concat(g);
        // msg.concat(" , B:");
        // msg.concat(b);

        String msg = "Display colour changed to ";
        msg.concat(backgroundColour);

        state.current(msg);
    }
}

void Display::printMessage(String line)
{
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
