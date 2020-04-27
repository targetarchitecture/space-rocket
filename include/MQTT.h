#ifndef MQTT_h
#define MQTT_h

#include <Arduino.h>
#include <list>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
//#include <fmt/format.h>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>
//#include <src/format.cc>

#include "credentials.h"

WiFiClient espClient;
PubSubClient MQTTClient(espClient);

extern void printMessage(String);
//extern String message;

// void MQTT_begin();
// void MQTT_loop();
void reconnect();
//void sendMessage(char *topic, String message);

template <typename... Args>
void sendMessage(const char *topic,const char* format, const Args & ... args);

void prvSendMessage(const char *topic,const char *message, fmt::format_args args);

void messageRecieved(char *topic, byte *payload, unsigned int length);
void handleEvents(String topic,String payload);
//void publishMQTTmessage(String msg);
void setupWifi();
void setupMQTTClient();

String topics[23] = {
    "sn1/sound/volume",
    "sn1/sound/state",
    "sn1/sound/busy",
    "sn1/sound/currentFile",
    "sn1/sound/play",
    "sn1/sound/action",
    "sn1/sound/read",
    "sn1/sound/currentFile",
    "sn1/touch/touched",
    "sn1/touch/released",
    "sn1/light/pin/0",
    "sn1/light/pin/1",
    "sn1/light/pin/2",
    "sn1/light/pin/3",
    "sn1/light/pin/4",
    "sn1/light/pin/5",
    "sn1/light/pin/6",
    "sn1/light/pin/7",
    "sn1/light/pin/8",
    "sn1/light/pin/9",
    "sn1/light/pin/10",
    "sn1/light/pin/11",
    "sn1/state"};

#endif
