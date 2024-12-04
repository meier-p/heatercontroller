// Module: message_module.cpp
// Purpose: Handles MQTT communication, including connecting to the broker, subscribing to topics, publishing messages, and processing incoming messages.
// Functions:
// - setupMQTT(): Initializes the MQTT client and connects to the broker.
// - reconnectMQTT(): Reconnects to the MQTT broker if the connection is lost.
// - setupMQTTSubscription(): Subscribes to necessary MQTT topics for control and updates.
// - sendKeepalive(): Sends a keepalive message to maintain subscriptions.
// - handleMQTTMessage(): Processes incoming MQTT messages and updates system state accordingly.
// - sendMessage(): Sends messages via MQTT, Telnet, or ESP logging based on priority and debug settings.


#include "message_module.h"
#include "heater_automation_module.h"
#include <ESPmDNS.h>
#include "data_module.h"
#include "gpio_module.h"
#include "i2c.h"
#include "ds18_module.h"
#include "firmware_update_module.h"
#include "esp_log.h" // Include ESP32 logging

static const char* TAG = "message_module"; // Define logging tag

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
String clientId = "heatercontroller";

// Function to set up MQTT communication
void setupMQTT() {
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(handleMQTTMessage);
    reconnectMQTT();
    setupMQTTSubscription();
    sendKeepalive();

    // Send the target temperatures after startup
    for (int i = 0; i < 10; i++) {
        if (strlen(zones[i].name) > 0 && !isnan(zones[i].temperatureTarget)) {
            String path = String(MQTT_BASE_PATH) + "/" + String(zones[i].name) + "/target_temperature";
            sendMessage(String(zones[i].temperatureTarget), path, 1);
        }
    }
}

// Function to reconnect to the MQTT broker
void reconnectMQTT() {
    if (!mqttClient.connected()) {
        sendMessage("Connecting to MQTT...", "debug", 6);
        if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
            sendMessage("MQTT connected", "debug", 6);
            setupMQTTSubscription();
            sendKeepalive();
        } else {
            sendMessage("MQTT connection failed with state " + String(mqttClient.state()), "debug", 6);
        }
    }
}

// Function to set up MQTT subscriptions
void setupMQTTSubscription() {
    // Subscribe to heater automation mode topic
    String heaterAutomationModeTopic = "N/" + String(MQTT_BASE_PATH) + "/heater_automation_mode";
    mqttClient.subscribe(heaterAutomationModeTopic.c_str(), 1); // Set QoS to 1
    sendMessage("Subscribed to topic: " + heaterAutomationModeTopic, "debug", 6);

    // Subscribe to valve mode topic
    String valveModeTopic = "N/" + String(MQTT_BASE_PATH) + "/valve_mode";
    mqttClient.subscribe(valveModeTopic.c_str(), 1); // Set QoS to 1
    sendMessage("Subscribed to topic: " + valveModeTopic, "debug", 6);

    // Subscribe to target temperature topics for each zone
    for (int i = 0; i < 10; i++) {
        if (strlen(zones[i].name) > 0) {
            String topic = "N/" + String(MQTT_BASE_PATH) + "/" + String(zones[i].name) + "/target_temperature";
            mqttClient.subscribe(topic.c_str(), 1); // Set QoS to 1
            sendMessage("Subscribed to topic: " + topic, "debug", 6);
        }
    }

    // Subscribe to heater toggle topic
    String heaterToggleTopic = "N/" + String(MQTT_BASE_PATH) + "/toggle";
    mqttClient.subscribe(heaterToggleTopic.c_str(), 1); // Set QoS to 1
    sendMessage("Subscribed to topic: " + heaterToggleTopic, "debug", 6);

    // Subscribe to sensor ID request topic
    String sensorIDsRequestTopic = "N/" + String(MQTT_BASE_PATH) + "/sensor_ids_request";
    mqttClient.subscribe(sensorIDsRequestTopic.c_str(), 1); // Set QoS to 1
    sendMessage("Subscribed to topic: " + sensorIDsRequestTopic, "debug", 6);

    // Subscribe to firmware update topic
    String firmwareUpdateTopic = "N/" + String(MQTT_BASE_PATH) + "/firmware_update";
    mqttClient.subscribe(firmwareUpdateTopic.c_str(), 1); // Set QoS to 1
    sendMessage("Subscribed to topic: " + firmwareUpdateTopic, "debug", 6);
}

// Function to send a keepalive message
void sendKeepalive() {
    String topic = "R/signalk/" + String(SYSTEM_ID) + "/keepalive";
    String payload = "[\"vessels/self/heater/+/target_temperature\", \"vessels/self/heater/toggle\", \"vessels/self/heater/heater_automation_mode\", \"vessels/self/heater/valve_mode\"]";
    mqttClient.publish(topic.c_str(), payload.c_str(), true);
    sendMessage("Sent keepalive to topic: " + topic, "debug", 6);
}

// Callback function to handle incoming MQTT messages
void handleMQTTMessage(char* topic, byte* payload, unsigned int length) {
    // Convert payload to String
    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    message.trim();

    // Output received message
    sendMessage("Received MQTT message on topic: " + String(topic), "debug", 6);
    sendMessage("Received payload: " + message, "debug", 6);

    // Parse JSON payload
    DynamicJsonDocument doc(256);

    // Handle firmware update request
    String firmwareUpdateTopic = "N/" + String(MQTT_BASE_PATH) + "/firmware_update";
    if (String(topic) == firmwareUpdateTopic) {
        DeserializationError error = deserializeJson(doc, message);
        if (error) {
            sendMessage("Failed to parse JSON payload", "debug", 6);
            return;
        }

        const char* updateUrl = doc["url"];
        if (updateUrl != nullptr && String(updateUrl).startsWith("http")) {
            sendMessage("Firmware update URL received: " + String(updateUrl), "debug", 6);
            checkForFirmwareUpdate(String(updateUrl).c_str(), 0);
        } else {
            sendMessage("Invalid firmware update URL in payload. Ensure it starts with 'http'.", "debug", 6);
        }

        return;
    }

    DeserializationError error = deserializeJson(doc, message);
    if (error) {
        sendMessage("Failed to parse JSON payload", "debug", 6);
        return;
    }

    // Extract value from JSON
    float value = doc["value"].as<float>();

    // Handle heater toggle message
    String heaterToggleTopic = "N/" + String(MQTT_BASE_PATH) + "/toggle";
    if (String(topic) == heaterToggleTopic) {
        if (value == 1) {
            toggleHeater(true);
            sendMessage("Heater toggled ON", "debug", 6);
        } else if (value == 0) {
            toggleHeater(false);
            sendMessage("Heater toggled OFF", "debug", 6);
        }
        return;
    }

    // Handle heater automation mode message
    String heaterAutomationModeTopic = "N/" + String(MQTT_BASE_PATH) + "/heater_automation_mode";
    if (String(topic) == heaterAutomationModeTopic) {
        if (value == 1) {
            automationActive = true;
            sendMessage("Heater automation mode set to ON", "debug", 6);
        } else if ((value == 0)) {
            automationActive = false;
            sendMessage("Heater automation mode set to OFF", "debug", 6);
        } else {
            sendMessage("Invalid heater automation mode command: " + message, "debug", 6);
        }
        return;
    }

    // Handle valve mode message
    String valveModeTopic = "N/" + String(MQTT_BASE_PATH) + "/valve_mode";
    if (String(topic) == valveModeTopic) {
        if (value == 1) {
            valveModeProportional = true;
            sendMessage("Valve mode set to PROPORTIONAL", "debug", 6);
        } else if (value == 0) {
            valveModeProportional = false;
            sendMessage("Valve mode set to ON/OFF", "debug", 6);
        } else {
            sendMessage("Invalid valve mode command: " + message, "debug", 6);
        }
        return;
    }

    // Handle sensor ID request
    String sensorIDsRequestTopic = "N/" + String(MQTT_BASE_PATH) + "/sensor_ids_request";
    if (String(topic) == sensorIDsRequestTopic) {
        String sensorIDs = "";
        for (int i = 0; i < BME680_SENSOR_COUNT; i++) {
            if (bme680Sensors[i].begin(bme680Addresses[i])) {
                sensorIDs += "BME680 Sensor " + String(i) + ": " + String(bme680Addresses[i], HEX) + "; ";
            }
        }
        for (int i = 0; i < numConnectedSensors; i++) {
            sensorIDs += "DS18 Sensor " + String(i) + ": ";
            for (uint8_t j = 0; j < 8; j++) {
                sensorIDs += String(connectedAddresses[i][j], HEX);
            }
            sensorIDs += "; ";
        }
        sendMessage(sensorIDs, "W/" + String(MQTT_BASE_PATH) + "/sensor_ids_response", 1);
        return;
    }

    // Process target temperature messages
    if (String(topic).endsWith("/target_temperature")) {
        sendMessage("Processing target temperature payload: " + String(value), "debug", 6);
        processIncomingData(String(topic), String(value));
    }
}

// Function to send messages via MQTT, Telnet, or ESP logging based on priority
void sendMessage(const String &message, const String &path, int priority) {

    if (DEBUG_MODE) {
        esp_log_level_set(TAG, ESP_LOG_DEBUG);
    } else {
        esp_log_level_set(TAG, ESP_LOG_WARN);
    }

    if (priority >= 6 && !DEBUG_MODE) {
        return;  // Skip debug messages if DEBUG_MODE is off
    }

    // MQTT output
    if (priority == 1 || priority == 2) {
        if (!mqttClient.connected()) {
            reconnectMQTT();
            sendMessage("MQTT not connected. Debug message sent.", "debug", 6);
        }
        if (mqttClient.connected()) {
            String modifiedPath = "W/" + path;
            mqttClient.publish(modifiedPath.c_str(), message.c_str(), true);  // Set retain flag and QoS to 1
        }
    }

    if (priority == 1) {
        return;
    }

    // Telnet output
    if (priority == 2 || priority == 6) {
        if (TelnetStream.available()) {
            TelnetStream.println(message);
        }
    }

    // Use ESP32 logging
    if (priority == 6) {
        ESP_LOGI(TAG, "%s", message.c_str());  // Info-level log
    }
}
