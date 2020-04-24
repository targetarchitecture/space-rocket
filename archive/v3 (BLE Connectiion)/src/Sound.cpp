#include <Arduino.h>
#include "Sound.h"

void Sound::begin()
{
  //Configure serial port pins and busy pin
  Serial1.begin(9600, SERIAL_8N1, 32, 33);

  pinMode(busyPin, INPUT);

    delay(3000); //For DFplayer?


  if (!myDFPlayer.begin(Serial1))
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
  bool currentBusy = busy();

  if (currentBusy != previousBusy)
  {
    String response = "";

    response.concat("{sound,busy,");
    response.concat(currentBusy);
    response.concat("}");

    sendMessage(response);

    previousBusy = currentBusy;
  }
}

bool Sound::busy()
{
  bool state = false;

  if (digitalRead(busyPin) == 1)
  {
    state = false;
  }
  else
  {
    state = true;
  }

  return state;
}

String Sound::execute(std::list<String> values)
{
  String function = values.front();
  values.pop_front();

  String response = "";

  if (function.equalsIgnoreCase("volume"))
  {
    int volume = values.front().toInt();
    volume = min(volume, 30); //Set volume value (0~30).

    myDFPlayer.volume(volume);

    response.concat("{sound,volume,");
    response.concat(volume);
    response.concat("}");
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

    response.concat("{sound,volume,");
    response.concat(myDFPlayer.readVolume());
    response.concat("}");
  }

  if (function.equalsIgnoreCase("readCurrentFileNumber"))
  {
    response.concat("{sound,currentFileNumber,");
    response.concat(myDFPlayer.readCurrentFileNumber());
    response.concat("}");
  }

  if (function.equalsIgnoreCase("volumeUp"))
  {
    myDFPlayer.volumeUp();

    response.concat("{sound,volume,");
    response.concat(myDFPlayer.readVolume());
    response.concat("}");
  }

  if (function.equalsIgnoreCase("busy"))
  {
    response.concat("{sound,busy,");
    response.concat(busy());
    response.concat("}");
  }

  if (function.equalsIgnoreCase("readState"))
  {
    response.concat("{sound,state,");
    response.concat(myDFPlayer.readState());
    response.concat("}");
  }

  if (function.equalsIgnoreCase("readVolume"))
  {
    int volume = myDFPlayer.readVolume();

    response.concat("{volume,");
    response.concat(volume);
    response.concat("}");
  }

  return response;
}
