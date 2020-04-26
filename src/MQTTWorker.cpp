#include <Arduino.h>
#include "MQTTWorker.h"

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
            MQTTClient.subscribe(MQTT_ACTION_TOPIC);
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
        MQTTClient.subscribe(MQTT_ACTION_TOPIC);
        // MQTTClient.subscribe(MQTT_VOLUME_TOPIC);
        // MQTTClient.subscribe(MQTT_STOP_TOPIC);
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
