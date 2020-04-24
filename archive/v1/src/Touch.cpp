#include <Arduino.h>
#include "Touch.h"

#define INFO // to see serial output of loop

extern void sendMessage(String);

void Touch::begin()
{
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  if (!cap.begin(0x5A))
  {
    Serial.println("MPR121 not found!");
  }
  else
  {
    Serial.println("MPR121 found.");
  }
}

void Touch::loop()
{
  // Get the currently touched pads
  currtouched = cap.touched();

  for (uint8_t i = 0; i < 12; i++)
  {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)))
    {
      String response = "";

      response.concat("{touch,touched,");
      response.concat(i);
      response.concat("}");

      sendMessage(response);
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)))
    {
      String response = "";

      response.concat("{touch,released,");
      response.concat(i);
      response.concat("}");

      sendMessage(response);
    }
  }

  // reset our state
  lasttouched = currtouched;
}
