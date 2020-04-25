#ifndef MQTTWorker_h
#define MQTTWorker_h

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>

#include "credentials.h"

WiFiClient espClient;
PubSubClient MQTTClient(espClient);

extern void printMessage(String);
extern String message;

// void MQTT_begin();
// void MQTT_loop();
void reconnect();
void sendMessage(String message);
void messageRecieved(char *topic, byte *payload, unsigned int length);
//void publishMQTTmessage(String msg);
void setupWifi();
void setupMQTTClient();

#endif
