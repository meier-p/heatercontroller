// Module: main.cpp
// Purpose: Main entry point for the program; coordinates initialization and the main control loop.
// Functions:
// - setup(): Initializes WiFi, OTA updates, sensors, MQTT, servos, and other modules.
// - loop(): Contains the main control logic, including reading sensor data, updating MQTT messages, handling automation, and checking for updates.


#include <Arduino.h>
#include "ota_module.h"
#include <ESPmDNS.h>
#include "message_module.h"
#include "dht_module.h"
#include "ds18_module.h"
#include "data_module.h"
#include "config.h"
#include "gpio_module.h"
#include "i2c.h"
#include "mcp41hv51_module.h"
#include "firmware_update_module.h"
#include "temperature_module.h"
#include "heater_automation_module.h"
#include "servo_control_module.h"
#include <esp_system.h> // For ESP32-specific functions

// Instantiate MCP41HV51 digital potentiometer on Chip Select pin 14
MCP41HV51 mcp41hv51(14);

// Variables for memory check intervals
unsigned long previousMemoryCheck = 0;
const long memoryCheckInterval = 5000; // Check every 5 seconds

// Variables for heater check intervals
unsigned long previousHeaterCheck = 0;
const long heaterCheckInterval = 5000; // Check every 5 seconds

// Function to monitor and print free memory
void printFreeMemory() {
    size_t free_heap = esp_get_free_heap_size();
    size_t min_free_heap = esp_get_minimum_free_heap_size();

    Serial.printf("Free Heap: %d bytes\n", free_heap);
    Serial.printf("Minimum Free Heap since start: %d bytes\n", min_free_heap);
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for the serial port to connect
    }

    // Initialize mDNS responder
    if (!MDNS.begin("esp32")) {
        sendMessage("Error starting MDNS responder!", "debug", 6);
    } else {
        sendMessage("MDNS responder started", "debug", 6);
    }

    // Set up WiFi, OTA updates, and sensors
    setupWiFi();
    setupOTA();
    setupDHT();
    setupDS18();
    setupGPIO();
    setupBME680();
    TelnetStream.begin();

    // Synchronize time using NTP
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    while (time(nullptr) < 8 * 3600 * 2) {
        delay(500);
    }

    // Initialize MCP41HV51
    mcp41hv51.begin();

    // Read initial sensor values and assign them to zones
    float sensors[NUM_SENSORS] = {0};
    float hums[NUM_SENSORS] = {0};
    float pressures[NUM_SENSORS] = {0};
    float vocs[NUM_SENSORS] = {0};
    readDHT(sensors[0], hums[0], sensors[1], hums[1], sensors[2], hums[2], sensors[3], hums[3], sensors[4], hums[4]);
    readDS18(sensors);
    readBME680(sensors, hums, pressures, vocs);
    assignSensorValues(sensors, hums, pressures, vocs);

    // Set up MQTT communication
    setupMQTT();
    setupMQTTSubscription();

    // Send the current modes of heater automation and valve mode
    String heaterAutomationPath = String(MQTT_BASE_PATH) + "/heater_automation_mode";
    sendMessage(automationActive ? "1" : "0", heaterAutomationPath, 1);
    String valveModePath = String(MQTT_BASE_PATH) + "/valve_mode";
    sendMessage(valveModeProportional ? "1" : "0", valveModePath, 1);

    // Send sensor information once after initialization
    getDS18SensorInfo();

    // Determine the main temperature after initialization
    determineMainTemperature();

    // Set up servo motors
    setupServos();
}

void loop() {
    static unsigned long lastPingTime = 0;
    static unsigned long lastUpdateTime = 0;
    static float lastMainTemperature = NAN; // To track the last sent value
    unsigned long currentMillis = millis();

    // Send a ping message every 30 seconds
    if (currentMillis - lastPingTime >= 30000) {
        lastPingTime = currentMillis;
        sendKeepalive(); // Keepalive message to maintain subscriptions
        sendMessage("Ping", "ping/path", 6);
    }

    ArduinoOTA.handle();
    mqttClient.loop();

    // Read sensor data
    float sensors[NUM_SENSORS];
    float hums[NUM_SENSORS];
    float pressures[NUM_SENSORS];
    float vocs[NUM_SENSORS];
    readDHT(sensors[0], hums[0], sensors[1], hums[1], sensors[2], hums[2], sensors[3], hums[3], sensors[4], hums[4]);
    readDS18(sensors);
    readBME680(sensors, hums, pressures, vocs);
    readHeaterStatus();

    // Assign sensor values to zones based on configuration
    assignSensorValues(sensors, hums, pressures, vocs);

    // Update data every 10 seconds
    if (currentMillis - lastUpdateTime >= 10000) {
        lastUpdateTime = currentMillis;

        // Send temperature and other data for each zone
        for (int i = 0; i < NUM_ZONES; i++) {
            if (strlen(zones[i].name) > 0) {
                // Send temperature
                if (!isnan(zones[i].temperature)) {
                    static float lastTemperature[NUM_ZONES] = {NAN};
                    if (zones[i].temperature != lastTemperature[i]) {
                        lastTemperature[i] = zones[i].temperature;
                        String path = String(MQTT_BASE_PATH) + "/" + String(zones[i].name) + "/temperature";
                        sendMessage(String(zones[i].temperature), path, 1);
                    }
                }
                // Send humidity
                if (!isnan(zones[i].humidity)) {
                    static float lastHumidity[NUM_ZONES] = {NAN};
                    if (zones[i].humidity != lastHumidity[i]) {
                        lastHumidity[i] = zones[i].humidity;
                        String path = String(MQTT_BASE_PATH) + "/" + String(zones[i].name) + "/humidity";
                        sendMessage(String(zones[i].humidity), path, 1);
                    }
                }
                // Send target temperature
                if (!isnan(zones[i].temperatureTarget)) {
                    static float lastTemperatureTarget[NUM_ZONES] = {NAN};
                    if (zones[i].temperatureTarget != lastTemperatureTarget[i]) {
                        lastTemperatureTarget[i] = zones[i].temperatureTarget;
                        String path = String(MQTT_BASE_PATH) + "/" + String(zones[i].name) + "/target_temperature";
                        sendMessage(String(zones[i].temperatureTarget), path, 1);
                    }
                }
                // Send pressure
                if (!isnan(zones[i].pressure)) {
                    static float lastPressure[NUM_ZONES] = {NAN};
                    if (zones[i].pressure != lastPressure[i]) {
                        lastPressure[i] = zones[i].pressure;
                        String path = String(MQTT_BASE_PATH) + "/" + String(zones[i].name) + "/pressure";
                        sendMessage(String(zones[i].pressure), path, 1);
                    }
                }
                // Send VOC value
                if (!isnan(zones[i].voc)) {
                    static float lastVOC[NUM_ZONES] = {NAN};
                    if (zones[i].voc != lastVOC[i]) {
                        lastVOC[i] = zones[i].voc;
                        String path = String(MQTT_BASE_PATH) + "/" + String(zones[i].name) + "/voc";
                        sendMessage(String(zones[i].voc), path, 1);
                    }
                }
                // Send valve position
                if (zones[i].servoValve > 0) {
                    static int lastValvePosition[NUM_ZONES] = {-1};
                    int valvePosition = getServoPosition(i);
                    if (valvePosition != lastValvePosition[i]) {
                        lastValvePosition[i] = valvePosition;
                        String path = String(MQTT_BASE_PATH) + "/" + String(zones[i].name) + "/valve_position";
                        sendMessage(String(valvePosition), path, 1);
                    }
                }
            }
        }

        // Determine the main temperature
        determineMainTemperature();

        // Send the main temperature only if it has changed
        if (!isnan(mainTemperature)) {
            if (isnan(lastMainTemperature) || fabs(mainTemperature - lastMainTemperature) > 0.01) {
                lastMainTemperature = mainTemperature;
                String path = String(MQTT_BASE_PATH) + "/main_temperature";
                sendMessage(String(mainTemperature), path, 1);
            }
        }

        // Send heater status if it has changed
        static bool lastHeaterStatus = false;
        if (heaterStatus != lastHeaterStatus) {
            lastHeaterStatus = heaterStatus;
            String path = String(MQTT_BASE_PATH) + "/status";
            sendMessage(String(heaterStatus ? 1 : 0), path, 1);
        }
    }

    // Heater automation based on zone temperatures, checked every 5 seconds
    if (currentMillis - previousHeaterCheck >= heaterCheckInterval) {
        previousHeaterCheck = currentMillis;
        controlHeaterBasedOnZones();
        controlServoValvesBasedOnZones(); // Control servo valves based on zones
    }

    // Check for Serial input to set resistance (for testing purposes)
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove whitespace and newlines
        int resistance = input.toInt();

        if (resistance >= 0 && resistance <= 255) {
            mcp41hv51.setResistance(resistance);
        } else {
            Serial.println("Invalid value. Please enter a number between 0 and 255.");
        }
    }

    // Memory monitoring
    if (currentMillis - previousMemoryCheck >= memoryCheckInterval) {
        previousMemoryCheck = currentMillis;
        printFreeMemory();
    }
}
