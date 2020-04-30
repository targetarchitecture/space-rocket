#include <Arduino.h>
#include "Sound.h"
#include <string>

void Sound::begin()
{
  //Configure serial port pins and busy pin
  Serial1.begin(9600, SERIAL_8N1, 32, 33);

  pinMode(busyPin, INPUT);

  delay(waitPeriod); //For DFplayer?

  bool isAck =  true;  
  bool performReset =  false;
  long timeoutDuration = 750;

  if (!myDFPlayer.begin(Serial1, isAck, performReset))
  {
    printMessage("DFplayer ERROR");
    delay(500);
    ESP.restart();
  }
  else
  {
    myDFPlayer.setTimeOut(timeoutDuration); //Set serial communictaion time out 500ms
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

Serial.print(myDFPlayer.available());

 // if (myDFPlayer.available() ) {
    detail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
 // }

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

//see if this works
   myDFPlayer.volume(20);  //Set volume value. From 0 to 30
   delay(200);

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

void Sound::detail(uint8_t type, int value)
{
Serial.print(type);

 switch (type) {
   case TimeOut:
     Serial.println(F("Time Out!"));
     break;
   case WrongStack:
     Serial.println(F("Stack Wrong!"));
     break;
   case DFPlayerCardInserted:
     Serial.println(F("Card Inserted!"));
     break;
   case DFPlayerCardRemoved:
     Serial.println(F("Card Removed!"));
     break;
   case DFPlayerCardOnline:
     Serial.println(F("Card Online!"));
     break;
   case DFPlayerPlayFinished:
     Serial.print(F("Number:"));
     Serial.print(value);
     Serial.println(F(" Play Finished!"));
     break;
   case DFPlayerError:
     Serial.print(F("DFPlayerError:"));
     switch (value) {
       case Busy:
         Serial.println(F("Card not found"));
         break;
       case Sleeping:
         Serial.println(F("Sleeping"));
         break;
       case SerialWrongStack:
         Serial.println(F("Get Wrong Stack"));
         break;
       case CheckSumNotMatch:
         Serial.println(F("Check Sum Not Match"));
         break;
       case FileIndexOut:
         Serial.println(F("File Index Out of Bound"));
         break;
       case FileMismatch:
         Serial.println(F("Cannot Find File"));
         break;
       case Advertise:
         Serial.println(F("In Advertise"));
         break;
       default:
         break;
     }
     break;
   default:
     break;
 }
}