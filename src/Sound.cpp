#include <Arduino.h>
#include "Sound.h"

void Sound::begin()
{
  //Configure serial port pins and busy pin
  Serial1.begin(9600, SERIAL_8N1, 32, 33);

  pinMode(busyPin, INPUT);

  delay(waitPeriod); //For DFplayer?

  if (!myDFPlayer.begin(Serial1,true,false))
  {
    printMessage("DFplayer ERROR");
    delay(500);
    ESP.restart();
  }
  else
  {
    myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

    fileCounts = myDFPlayer.readFileCounts();

    if (fileCounts == -1)
    {
      printMessage("DFplayer NO FILES FOUND");
      delay(500);
      ESP.restart();
    }
    else
    {
      printMessage("Found: Sound Effects");

      char buffer[50];
      sprintf(buffer, "SD Card: %i files", fileCounts);
      String msg = buffer;

      printMessage(msg);
    }
  }
}

void Sound::loop()
{
  int currentBusy = busy();

  if (currentBusy != previousBusy)
  {
    String response = "";

    response.concat("{sound,busy,");
    response.concat(currentBusy);
    response.concat("}");

    sendDataToMicrobit(response);

    previousBusy = currentBusy;
  }
}

int Sound::busy()
{
  int state = -1;

  if (digitalRead(busyPin) == 1)
  {
    state = 0;
  }
  else
  {
    state = 1;
  }

  return state;
}

void Sound::execute(std::list<String> values)
{
  String function = values.front();
  values.pop_front();

  if (function.equalsIgnoreCase("volume"))
  {
    int volume = values.front().toInt();
    volume = min(volume, 30); //Set volume value (0~30).

    myDFPlayer.volume(volume);

    String response = "";

    response.concat("{sound,volume,");
    response.concat(volume);
    response.concat("}");

    sendDataToMicrobit(response);
  }

  if (function.equalsIgnoreCase("play"))
  {
    int fileNumber = values.front().toInt();
    myDFPlayer.play(fileNumber);
  }

  if (function.equalsIgnoreCase("previous"))
  {
    myDFPlayer.previous();
  }

  if (function.equalsIgnoreCase("next"))
  {
    myDFPlayer.next();
  }

  if (function.equalsIgnoreCase("pause"))
  {
    myDFPlayer.pause();
  }

  if (function.equalsIgnoreCase("stop"))
  {
    myDFPlayer.stop();
  }

  if (function.equalsIgnoreCase("start"))
  {
    myDFPlayer.start();
  }

  if (function.equalsIgnoreCase("volumeDown"))
  {
    myDFPlayer.volumeDown();

    String response = "";

    response.concat("{sound,volume,");
    response.concat(myDFPlayer.readVolume());
    response.concat("}");

    sendDataToMicrobit(response);
  }

  if (function.equalsIgnoreCase("readCurrentFileNumber"))
  {
    String response = "";

    response.concat("{sound,currentFileNumber,");
    response.concat(myDFPlayer.readCurrentFileNumber());
    response.concat("}");

    sendDataToMicrobit(response);
  }

  if (function.equalsIgnoreCase("volumeUp"))
  {
    myDFPlayer.volumeUp();

    String response = "";

    response.concat("{sound,volume,");
    response.concat(myDFPlayer.readVolume());
    response.concat("}");

    sendDataToMicrobit(response);
  }

  if (function.equalsIgnoreCase("busy"))
  {
    String response = "";

    response.concat("{sound,busy,");
    response.concat(busy());
    response.concat("}");

    sendDataToMicrobit(response);
  }

  if (function.equalsIgnoreCase("readState"))
  {
    String response = "";

    response.concat("{sound,state,");
    response.concat(myDFPlayer.readState());
    response.concat("}");

    sendDataToMicrobit(response);
  }

  if (function.equalsIgnoreCase("readVolume"))
  {
    int volume = myDFPlayer.readVolume();

    String response = "";

    response.concat("{volume,");
    response.concat(volume);
    response.concat("}");

    sendDataToMicrobit(response);
  }
}
