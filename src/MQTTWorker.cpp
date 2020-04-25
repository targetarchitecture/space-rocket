#include <Arduino.h>
#include "MQTTWorker.h"

void MQTTWorker::begin()
{
    btStop(); // turn off bluetooth (part of ESP32 core)

    printMessage("Setup Encoders");

    String response = "";

    response.concat("Setup DAC (");
    response.concat(DAC2);
    response.concat(")");

    printMessage(response);
}

void MQTTWorker::loop()
{
}


void setupWifi()
{
  //sort out WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to the network

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready on the local network");
  Serial.println("IP address: " + WiFi.localIP().toString());
}