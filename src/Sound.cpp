#include <Arduino.h>
#include "Sound.h"
#include <string>

void Sound::begin()
{
  //Configure serial port pins and busy pin
  Serial1.begin(9600, SERIAL_8N1, 32, 33);

  pinMode(busyPin, INPUT);

  delay(waitPeriod); //For DFplayer?

  if (!myDFPlayer.begin(Serial1, true, false))
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
    String message = (String) currentBusy;

    sendMessage("sn1/sound/busy", message.c_str());

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

void Sound::execute(String topic, String payload)
{
  String function = payload;

  if (function.equalsIgnoreCase("volume"))
  {
    int volume = payload.toInt();
    volume = constrain(volume, 0, 30); //Set volume value (0~30).

    myDFPlayer.volume(volume);

    String message = (String)volume;

    sendMessage("sn1/sound/volume" , message.c_str());

  }

  if (function.equalsIgnoreCase("play"))
  {
    int fileNumber = payload.toInt();
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

   String volume =  (String)myDFPlayer.readVolume();

    sendMessage("sn1/sound/volume", volume.c_str());
  }

  if (function.equalsIgnoreCase("readCurrentFileNumber"))
  {
    // String response = "";

    // response.concat("{sound,currentFileNumber,");
    // response.concat(myDFPlayer.readCurrentFileNumber());
    // response.concat("}");

    // sendMessage(response);

//     string message = fmt::format("%s", myDFPlayer.readCurrentFileNumber());

// message.

String message = (String)myDFPlayer.readCurrentFileNumber();

    sendMessage("sn1/sound/currentFile", message.c_str());
  }

  if (function.equalsIgnoreCase("volumeUp"))
  {
    myDFPlayer.volumeUp();

    String message = (String)myDFPlayer.readVolume();

    sendMessage("sn1/sound/volume", message.c_str());
  }

  if (function.equalsIgnoreCase("read busy"))
  {
    String message = (String) busy();

    sendMessage("sn1/sound/volume", message.c_str());
  }

  if (function.equalsIgnoreCase("readState"))
  {
    String message =(String)myDFPlayer.readState();

    sendMessage("sn1/sound/state", message.c_str());
  }

  if (function.equalsIgnoreCase("read Volume"))
  {
    String message = (String) myDFPlayer.readVolume();

    sendMessage("sn1/sound/volume", message.c_str());
  }
}
