#include <Arduino.h>
#include "MQTT.h"

void MQTT_begin()
{
    btStop(); // turn off bluetooth (part of ESP32 core)

    setupWifi();
    setupMQTTClient();

    sendMessage("sn1/state", "IP address: ", WiFi.localIP().toString());
}

void sendMessage(String topic, String message, String arg1)
{
    Serial.printf("Sending to MQTT: %s\n", message.c_str());

    char buffer[50];
    sprintf(buffer, message.c_str(), arg1);

    MQTTClient.publish(topic.c_str(), buffer);
}

void sendMessage(String topic, String message)
{
    Serial.printf("Sending to MQTT: %s\n", message.c_str());

    MQTTClient.publish(topic.c_str(), message.c_str());
}

void sendMessage(String topic, uint8_t message)
{
    String str = (String)message;

    Serial.printf("Sending to MQTT: %s\n", str.c_str());

    MQTTClient.publish(topic.c_str(), str.c_str());
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
            MQTTClient.publish("sn1/state", "Reconnected");

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
        MQTTClient.publish("sn1/state", "Connected to MQTT server");

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
    //Serial.printf("Message arrived [%s]\n", topic);

    String builtMessage = "";

    for (int i = 0; i < length; i++)
    {
        builtMessage += (char)payload[i];
    }

    //Serial.println(builtMessage);

    handleEvents(topic, builtMessage);
}

