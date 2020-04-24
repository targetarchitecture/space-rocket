#include "Arduino.h"
#include "BLEDevice.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e");

// The characteristic of the remote service we are interested in.
static BLEUUID charRXUUID("6e400002-b5a3-f393-e0a9-e50e24dcca9e");
static BLEUUID charTXUUID("6e400003-b5a3-f393-e0a9-e50e24dcca9e");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic *pRemoteCharacteristicRX;
static BLERemoteCharacteristic *pRemoteCharacteristicTX;
static BLEAdvertisedDevice *myDevice;

extern String message;
//String partialMessage = ""; // a string to hold incoming data

void BLESerial_loop();
void getSerialMessage();
void sendMessage(String);
bool connectToMicrobit();

static void recieveMicrobitBLEmessage(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
  Serial.print("Notify callback for RX characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
  // Serial.print("data: ");
  // Serial.println((char *)pData);

  String tmp = (char *)pData;

  tmp = tmp.substring(0, length);
  tmp.trim();

  Serial.print("cleaned data: ");
  Serial.println(tmp);

  message = tmp;
}

class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient *pclient)
  {
  }

  void onDisconnect(BLEClient *pclient)
  {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToMicrobit()
{
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient *pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice); // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr)
  {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristicTX = pRemoteService->getCharacteristic(charTXUUID);
  if (pRemoteCharacteristicTX == nullptr)
  {
    Serial.print("Failed to find our TX characteristic UUID: ");
    Serial.println(charTXUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our TX characteristic");

  pRemoteCharacteristicRX = pRemoteService->getCharacteristic(charRXUUID);
  if (pRemoteCharacteristicRX == nullptr)
  {
    Serial.print("Failed to find our RX characteristic UUID: ");
    Serial.println(charRXUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our RX characteristic");

  std::string value = pRemoteCharacteristicRX->readValue();
  Serial.print("The characteristic value was: ");
  Serial.println(value.c_str());

  Serial.print("The characteristic can indicate");
  Serial.println(pRemoteCharacteristicRX->canIndicate());

  pRemoteCharacteristicRX->registerForNotify(recieveMicrobitBLEmessage, false);

  connected = true;
  return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    //if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
    if (advertisedDevice.getAddress().toString() == "ea:9d:56:a3:44:8f")
    {
      Serial.println("BLE Advertised Device found: ea:9d:56:a3:44:8f");

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  }   // onResult
};    // MyAdvertisedDeviceCallbacks

void BLESerial_begin()
{
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("Ada's Super Computer");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.

// This is the Arduino main loop function.
void BLESerial_loop()
{
  getSerialMessage();

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true)
  {
    if (connectToMicrobit())
    {
      Serial.println("We are now connected to the BLE Server.");
    }
    else
    {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected)
  {
    // String newValue = "Time since boot: " + String(millis() / 1000);
    // Serial.println("Setting new characteristic value to \"" + newValue + "\"");

    // newValue = "P:\n";

    // // Set the characteristic's value to be the array of bytes that is actually a string.
    // pRemoteCharacteristicTX->writeValue(newValue.c_str(), newValue.length());
  }
  else if (doScan)
  {
    BLEDevice::getScan()->start(0); // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
}

void sendMessage(String message)
{
  //TODO: debate if it needs a line break!
  Serial.println(message);

  if (connected)
  {
    // String newValue = "Time since boot: " + String(millis() / 1000);
    // Serial.println("Setting new characteristic value to \"" + newValue + "\"");

    // newValue = "P:\n";

    // // Set the characteristic's value to be the array of bytes that is actually a string.
    // pRemoteCharacteristicTX->writeValue(newValue.c_str(), newValue.length());

    pRemoteCharacteristicTX->writeValue(message.c_str(), message.length());
  }

  try
  {
    Serial2.println(message);
  }
  catch (int e)
  {
    Serial.print("An exception occurred. Exception Nr. ");
    Serial.println(e, DEC);
  }
}

void getSerialMessage()
{
  String partialMessage = ""; // a string to hold incoming data

  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();

    //end of message character
    if (inChar == '}') // '\n')
    {
      message = partialMessage;

      // clear the string:
      partialMessage = "";
    }
    else
    {
      // add it to the inputString:
      partialMessage += inChar;
    }
  }

  // while (Serial2.available())
  // {
  //   // get the new byte:
  //   char inChar = (char)Serial2.read();

  //   //end of message character
  //   if (inChar == '}') // '\n')
  //   {
  //     message = partialMessage;

  //     // clear the string:
  //     partialMessage = "";
  //   }
  //   else
  //   {
  //     // add it to the inputString:
  //     partialMessage += inChar;
  //   }
  // }
}
