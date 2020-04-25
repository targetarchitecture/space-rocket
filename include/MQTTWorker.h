#ifndef MQTTWorker_h

#define MQTTWorker_h

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>

#include "credentials.h"

class MQTTWorker
{
public:
  void begin();
  void loop();

private:
  PubSubClient MQTTClient;
  void callback(char *topic, byte *payload, unsigned int length);
  void publishMQTTmessage(String msg);
  void setupWifi();
};

#endif
