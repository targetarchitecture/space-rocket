#include <Arduino.h>
#include "Sound.h"

using namespace std;

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
    string message = "";// fmt::format("%s", currentBusy);

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

    //std::string s = fmt::format("{sound,volume,%s}", volume);
    //sendMessage("sn1/sound/volume"  s.c_str());
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

    // String response = "";
    // response.concat("{sound,volume,");
    // response.concat(myDFPlayer.readVolume());
    // response.concat("}");

    string message =""; // fmt::format("%s", myDFPlayer.readVolume());

    sendMessage("sn1/sound/volume", message.c_str());
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

std::string message ="";// std::format("The answer is {}.", 42);

    sendMessage("sn1/sound/currentFile", message.c_str());
  }

  if (function.equalsIgnoreCase("volumeUp"))
  {
    myDFPlayer.volumeUp();

    // String response = "";

    // response.concat("{sound,volume,");
    // response.concat(myDFPlayer.readVolume());
    // response.concat("}");

    // sendMessage(response);

    string message = ""; //fmt::format("%s", myDFPlayer.readVolume());

    sendMessage("sn1/sound/volume", message.c_str());
  }

  if (function.equalsIgnoreCase("read busy"))
  {
    // String response = "";

    // response.concat("{sound,busy,");
    // response.concat(busy());
    // response.concat("}");

    // sendMessage(response);

    string message = ""; //fmt::format("%s", busy());

    sendMessage("sn1/sound/volume", message.c_str());
  }

  if (function.equalsIgnoreCase("readState"))
  {
    //     String response = "";

    //     response.concat("{sound,state,");
    //     response.concat(myDFPlayer.readState());
    //     response.concat("}");

    //  sn1/sound/state

    //     sendMessage(response);

    //    sendMessage("sn1/sound/state", "%s", myDFPlayer.readState());

    string message ="";// fmt::format("%s", myDFPlayer.readState());

    sendMessage("sn1/sound/state", message.c_str());
  }

  if (function.equalsIgnoreCase("read Volume"))
  {
    //int volume = myDFPlayer.readVolume();

    // String response = "";

    // response.concat("{volume,");
    // response.concat(volume);
    // response.concat("}");

    // sendMessage(response);

    string message = "";// fmt::format("%s", myDFPlayer.readVolume());

    sendMessage("sn1/sound/volume", message.c_str());
  }
}
