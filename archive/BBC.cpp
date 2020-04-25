void sendDataToMicrobit(String message);
void getDataFromMicrobit();

    Serial2.begin(28800);  //BBC Microbit Serial

    
    //clear the buffers to begin processing
    delay(10);
    Serial.flush();
    Serial2.flush();
    delay(10);


    void sendDataToMicrobit(String message)
{
    Serial.println("Sending to MicroBit:" + message);
    //Serial.println(message);
    //   Serial.flush();

    try
    {
        Serial2.print(message);
      
      //  Serial2.flush();
    }
    catch (int e)
    {
        Serial.print("An exception occurred sending to MicroBit. Exception Nr. ");
        Serial.println(e, DEC);
    }
}

//https://forum.arduino.cc/index.php?topic=396450.0
const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

void getDataFromMicrobit()
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '{';
    char endMarker = '}';
    char rc;

    while (Serial2.available() > 0 && newData == false)
    {
        rc = Serial2.read();

        if (recvInProgress == true)
        {
            if (rc != endMarker)
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars)
                {
                    ndx = numChars - 1;
                }
            }
            else
            {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker)
        {
            recvInProgress = true;
        }
    }

    if (newData == true)
    {
        //quick debug
        Serial.print("Recieved from MicroBit:");
        Serial.println(receivedChars);

        //set the message variable
        message = receivedChars;

        //reset to get next message
        newData = false;
    }
}