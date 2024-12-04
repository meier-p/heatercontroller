// Module: config.h
// Purpose: Contains configuration settings for WiFi, MQTT, debug modes, and zone definitions.
// Definitions:
// - WiFi credentials and MQTT server settings.
// - Number of zones and sensors, hysteresis values for temperature control.
// Structures:
// - Zone: Represents a heating zone with attributes like name, current temperature, target temperature, humidity, etc.
// External Variables:
// - zones[]: Array of Zone structures representing different heating zones.
// - mainTemperature: Global variable representing the main temperature used for control logic.

#ifndef CONFIG_H
#define CONFIG_H

// WiFi credentials
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"

// MQTT credentials
#define MQTT_HOST "your_mqtt_host"
#define MQTT_USER "your_mqtt_user"
#define MQTT_PASS "your_mqtt_password"
#define MQTT_PORT 1883
#define MQTT_BASE_PATH "signalk/your_system_id/vessels/self/heater"
#define SYSTEM_ID "your_system_id"

// Debug settings
#define DEBUG_MODE true  // Set to false to disable debug messages

// Define the number of zones
#define NUM_ZONES 10 // Adjust to the actual number of zones
#define NUM_SENSORS 20 // Max number of sensors (5 DHT, 10 DS18, 5 BME680)

// Hysteresis values for temperature control
#define HYSTERESIS_OVER 1.0  // Threshold above the target in degrees Celsius
#define HYSTERESIS_UNDER 2.0 // Threshold below the target in degrees Celsius

// Structure to represent a heating zone
struct Zone {
    char name[20];            // Name of the zone
    float temperature;        // Current temperature
    float humidity;           // Current humidity
    float temperatureTarget;  // Target temperature
    float temperatureValve;   // Temperature at the valve (if applicable)
    int servoValve;           // Servo valve number
    float pressure;           // Atmospheric pressure
    float voc;                // VOC value (air quality)
};

// Declare the global zones array as external
extern Zone zones[NUM_ZONES];

// Declare mainTemperature as external to make it accessible in other files
extern float mainTemperature;

#endif
