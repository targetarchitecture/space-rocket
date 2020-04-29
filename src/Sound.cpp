#include <Arduino.h>
#include "Sound.h"
#include <string>

void Sound::begin()
{
  //Configure serial port pins and busy pin
  Serial1.begin(9600, SERIAL_8N1, 32, 33);

  pinMode(busyPin, INPUT);

  delay(waitPeriod); //For DFplayer?

bool isAck = false;// true;

  if (!myDFPlayer.begin(Serial1, isAck, false))
  {
    printMessage("DFplayer ERROR");
    delay(500);
    ESP.restart();
  }
  else
  {
    myDFPlayer.setTimeOut(5000); //Set serial communictaion time out 500ms
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
    String message = (String)currentBusy;

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
  // Serial.print("Message arrived for sound [");
  // Serial.print(topic);
  // Serial.println("]");

  //make lower case
  topic.toLowerCase();

  if (topic.equalsIgnoreCase("sn1/sound/volume"))
  {
    if (payload.equalsIgnoreCase("up"))
    {
      myDFPlayer.volumeUp();
    }
    else if (payload.equalsIgnoreCase("down"))
    {
      myDFPlayer.volumeDown();
    }
    else
    {
      int volume = payload.toInt();
      volume = constrain(volume, 0, 30); //Set volume value (0~30).

      myDFPlayer.volume(volume);
    }
  }

  if (topic.equalsIgnoreCase("sn1/sound/play"))
  {
    if (payload.equalsIgnoreCase("previous"))
    {
      myDFPlayer.previous();
    }
    else if (payload.equalsIgnoreCase("next"))
    {
      myDFPlayer.next();
    }
    else if (payload.equalsIgnoreCase("start"))
    {
      myDFPlayer.start();
    }
    else if (payload.equalsIgnoreCase("pause"))
    {
      myDFPlayer.pause();
    }
    else if (payload.equalsIgnoreCase("stop"))
    {
      myDFPlayer.stop();
    }
    else
    {
      int fileNumber = payload.toInt();

      playTrack(fileNumber);
    }
  }

  if (topic.startsWith("sn1/sound"))
  {
    //add a delay to see if this stops the shutdowns
    delay(100);
  }

  // if (function.equalsIgnoreCase("readCurrentFileNumber"))
  // {
  //   String message = (String)myDFPlayer.readCurrentFileNumber();

  //   sendMessage("sn1/sound/currentFile", message.c_str());
  // }

  // if (function.equalsIgnoreCase("read busy"))
  // {
  //   String message = (String)busy();

  //   sendMessage("sn1/sound/volume", message.c_str());
  // }

  // if (function.equalsIgnoreCase("readState"))
  // {
  //   String message = (String)myDFPlayer.readState();

  //   sendMessage("sn1/sound/state", message.c_str());
  // }

  // if (function.equalsIgnoreCase("read Volume"))
  // {
  //   String message = (String)myDFPlayer.readVolume();

  //   sendMessage("sn1/sound/volume", message.c_str());
  // }
}

void Sound::playTrack(uint8_t track)
{
  Serial.print("Track:");
  Serial.println(track);

  myDFPlayer.stop();
  delay(200);
  myDFPlayer.play(track);
  delay(200);
  int file = myDFPlayer.readCurrentFileNumber();

  Serial.print("File:");
  Serial.println(file);

  while (file != track)
  {
    myDFPlayer.play(track);
    delay(200);
    file = myDFPlayer.readCurrentFileNumber();

    Serial.print("Looping File:");
    Serial.println(file);
  }
}
