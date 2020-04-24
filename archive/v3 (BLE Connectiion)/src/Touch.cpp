#include <Arduino.h>
#include "Touch.h"

#define INFO // to see serial output of loop

extern void sendMessage(String);
extern void printMessage(String);

void Touch::begin()
{
  if (!cap.begin(0x5A))
  {
    printMessage("MPR121 not found");
    delay(500);
    ESP.restart();
  }
  else
  {
    printMessage("Capacitive Touch found");
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

      response.concat("{t,touched,");
      response.concat(i);
      response.concat("}");

      sendMessage(response);

      char buffer[50];
      sprintf(buffer, "%i touched", i);
      String msg = buffer;
      printMessage(msg);
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)))
    {
      String response = "";

      response.concat("{t,released,");
      response.concat(i);
      response.concat("}");

      sendMessage(response);

      char buffer[50];
      sprintf(buffer, "%i released", i);
      String msg = buffer;
      printMessage(msg);
    }
  }

  // reset our state
  lasttouched = currtouched;
}
