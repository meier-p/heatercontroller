// Module: message_module.h
// Purpose: Declares functions and variables for message handling and MQTT communication.
// External Variables:
// - wifiClient: WiFi client for network communication.
// - mqttClient: MQTT client for messaging.
// Function Prototypes:
// - sendMessage()
// - setupMQTT()
// - reconnectMQTT()
// - setupMQTTSubscription()
// - handleMQTTMessage()
// - sendKeepalive()


#ifndef MESSAGE_MODULE_H
#define MESSAGE_MODULE_H

#include "config.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <TelnetStream.h>
#include <ArduinoJson.h>

// External declarations for WiFi and MQTT clients
extern WiFiClient wifiClient;
extern PubSubClient mqttClient;

// Function prototypes
void sendMessage(const String &message, const String &path, int priority);
void setupMQTT();
void reconnectMQTT();
void setupMQTTSubscription();
void handleMQTTMessage(char* topic, byte* payload, unsigned int length);
void sendKeepalive();

#endif
