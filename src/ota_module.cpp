// Module: ota_module.cpp
// Purpose: Manages Over-The-Air (OTA) updates and WiFi connection setup.
// Functions:
// - setupWiFi(): Connects to the specified WiFi network.
// - setupOTA(): Initializes OTA update functionality, allowing firmware updates over the network.


#include "ota_module.h"

// Function to set up WiFi connection
void setupWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    unsigned long startAttemptTime = millis();

    // Try to connect to WiFi within a timeout period
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 150000) { // Increased timeout to 150 seconds
        delay(500);
    }

    if (WiFi.status() != WL_CONNECTED) {
        // If connection fails, restart the ESP
        ESP.restart();
    }

    Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());
}

// Function to set up OTA updates
void setupOTA() {
    ArduinoOTA.setPort(8266);  // Set a specific OTA port (default is 8266)
    ArduinoOTA.setHostname("heatercontroller");  // Set a hostname for easier identification
    ArduinoOTA.setPassword("1234");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        // Serial.println("Start updating " + type);
    });

    ArduinoOTA.onEnd([]() {
        // Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            // Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            // Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            // Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            // Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            // Serial.println("End Failed");
        }
    });

    ArduinoOTA.setTimeout(120000);  // Set OTA timeout to 120 seconds
    ArduinoOTA.begin();
    // Serial.println("OTA Ready");
}
