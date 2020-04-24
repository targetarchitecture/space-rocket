#include <Arduino.h>
#include "Testing.h"

void Testing::setup()
{
 // tests.push_back("{sound,play,1}");
  // tests.push_back("{motion,setPWM,1,2540}");
  // tests.push_back("{motion,setPWM,1,380}");
 // tests.push_back("{feature,setDAC,1.2}");
  // tests.push_back("{motion,setPWM,1,2540}");
  // tests.push_back("{motion,setPWM,1,380}");
 // tests.push_back("{feature,setDAC,3.2}");
  //tests.push_back("{sound,play,3}");
  // tests.push_back("{motion,setPWM,1,2540}");
  // tests.push_back("{motion,setPWM,1,380}");

  // tests.push_back("{motion,startPosition,1,0}");
  // tests.push_back("{motion,moveTo,1,100,40}");
  // tests.push_back("{motion,moveTo,1,0,40}");

  numberOfTests = tests.size();
}

void Testing::loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    if (currentTest <= numberOfTests)
    {
      message = tests[currentTest];

      currentTest++;
    }
  }
}

