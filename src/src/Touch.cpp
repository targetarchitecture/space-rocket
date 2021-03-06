#include <Arduino.h>
#include "Touch.h"

#define INFO // to see serial output of loop

void Touch::begin()
{
  if (!cap.begin(0x5A))
  {
    printMessage("MPR121 not found");

    state.warning("MPR121 not found");

    touchWorking = false;
  }
  else
  {
    printMessage("Capacitive Touch found");

    state.current("Capacitive Touch found");

    touchWorking = true;
  }
}

void Touch::loop()
{
  if (touchWorking == false)
  {
    return;
  }

  // Get the currently touched pads
  currtouched = cap.touched();

  for (uint8_t pin = 0; pin < 12; pin++)
  {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(pin)) && !(lasttouched & _BV(pin)))
    {
      sendMessage("sn1/touch/touched", pin);

      printMessage("%i touched", pin);
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(pin)) && (lasttouched & _BV(pin)))
    {
      sendMessage("sn1/touch/released", pin);

      printMessage("%i released", pin);
    }
  }

  // reset our state
  lasttouched = currtouched;
}
