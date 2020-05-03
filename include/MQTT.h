#ifndef MQTT_h
#define MQTT_h

#include <Arduino.h>
#include <State.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

#include "credentials.h"

WiFiClient espClient;
PubSubClient MQTTClient(espClient);

extern void printMessage(String message);
extern void printMessage(String message, uint8_t arg1);
extern State state;

void reconnect();
void sendMessage(String topic, String message);
void sendMessage(String topic, String message, String arg1);
void sendMessage(String topic, uint8_t message);
void messageRecieved(char *topic, byte *payload, unsigned int length);
void handleEvents(String topic, String payload);
void setupWifi();
void setupMQTTClient();
void setupOTA();

String topics[7] = {
    "sn1/sound/volume",
    "sn1/sound/play",
    "sn1/light/blink",
    "sn1/light/breathe",
    "sn1/light/on",
    "sn1/light/off",
    "sn1/motion/pwm"};

#endif
