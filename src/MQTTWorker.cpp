#include <Arduino.h>
#include "MQTTWorker.h"

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

void MQTT_begin()
{
    btStop(); // turn off bluetooth (part of ESP32 core)

    setupWifi();
    setupMQTTClient();

    String SSID = WIFI_SSID;
    String mDNS = MDNS_HOSTNAME;
    String MQTTSVR = MQTT_SERVER;
    sendMessage("Connected to SSID: " + SSID);
    sendMessage("IP address: " + WiFi.localIP().toString());
    sendMessage("mDNS: " + mDNS);
    sendMessage("Connected to MQTT server: " + MQTTSVR);
}

void sendMessage(String message)
{
    Serial.println("Sending to MQTT:" + message);

    MQTTClient.publish(MQTT_TOPIC, message.c_str());
}

void setupWifi()
{
    //sort out WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to the network

    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        printMessage("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    printMessage("Ready on the local network");
    printMessage("IP address: " + WiFi.localIP().toString());
}

void reconnect()
{
    // Loop until we're reconnected
    while (!MQTTClient.connected())
    {
        yield();

        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = MQTT_CLIENTID;

        // Attempt to connect
        if (MQTTClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
        {
            Serial.println("connected");

            // Once connected, publish an announcement...
            MQTTClient.publish(MQTT_TOPIC, "Reconnected");

            // ... and resubscribe
            for (auto &&topic : topics)
            {
                MQTTClient.subscribe(topic.c_str());
            }
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(MQTTClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MQTT_loop()
{
    //MQTT section
    if (!MQTTClient.connected())
    {
        reconnect();
    }

    MQTTClient.loop();
}

void setupMQTTClient()
{
    Serial.println("Connecting to MQTT server");

    //MQTTClient.setClient(espClient);
    MQTTClient.setServer(MQTT_SERVER, 1883);

    // setup callbacks
    MQTTClient.setCallback(messageRecieved);

    Serial.println("connect mqtt...");

    String clientId = MQTT_CLIENTID;

    if (MQTTClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
    {
        Serial.println("Connected");
        MQTTClient.publish(MQTT_TOPIC, "Connected to MQTT server");

        Serial.println("subscribe");

            for (auto &&topic : topics)
            {
                MQTTClient.subscribe(topic.c_str());

                Serial.println("subscribed to:" + topic);
            }
    }
}

void messageRecieved(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    String partialMessage = "";

    for (int i = 0; i < length; i++)
    {
        partialMessage += (char)payload[i];
    }

    Serial.println(partialMessage);

    message = partialMessage;
}
