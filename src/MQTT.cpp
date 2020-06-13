#include <Arduino.h>
#include "MQTT.h"

void MQTT_begin()
{
    btStop(); // turn off bluetooth (part of ESP32 core)

    setupWifi();

    setupOTA();

    setupMQTTClient();

    state.ipAddress(WiFi.localIP().toString());
}

void sendMessage(String topic, String message, String arg1)
{
    //Serial.printf("Sending to MQTT: %s - %s\n", topic.c_str(), message.c_str());

    char buffer[50];
    sprintf(buffer, message.c_str(), arg1);

    MQTTClient.publish(topic.c_str(), buffer);
}

void sendMessage(String topic, String message)
{
    //Serial.printf("Sending to MQTT: %s - %s\n", topic.c_str(), message.c_str());

    MQTTClient.publish(topic.c_str(), message.c_str());
}

void sendMessage(String topic, uint8_t message)
{
    String str = (String)message;

    //Serial.printf("Sending to MQTT: %s - %s\n", topic.c_str(), str.c_str());

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

        //Serial.print("Attempting MQTT connection...");

        // Create a random client ID
        String clientId = MQTT_CLIENTID;

        // Attempt to connect
        if (MQTTClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
        {
            //Serial.println("connected");

            // Once connected, publish an announcement...
            state.warning("Had to reconnect to MQTT server");

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

            String msg = "Failed to reconnect to MQTT server with error:";
            msg.concat(MQTTClient.state());

            state.error(msg);

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

    ArduinoOTA.handle();
}

void setupMQTTClient()
{
    //Serial.println("Connecting to MQTT server");

    MQTTClient.setServer(MQTT_SERVER, 1883);

    // setup callbacks
    MQTTClient.setCallback(messageRecieved);

    //Serial.println("connect mqtt...");

    String clientId = MQTT_CLIENTID;

    if (MQTTClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_KEY))
    {
        //Serial.println("Connected");

        state.current("Connected to MQTT server");
        state.error("");
        state.warning("");

        //Serial.println("subscribe");

        for (auto &&topic : topics)
        {
            MQTTClient.subscribe(topic.c_str());

            //Serial.println("subscribed to:" + topic);
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

void setupOTA()
{
    ArduinoOTA.setHostname(MDNS_HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);

    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
            Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
                Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)
                Serial.println("End Failed");
        });

    ArduinoOTA.begin();
}
