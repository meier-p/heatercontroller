// Module: ds18_module.cpp
// Purpose: Manages DS18B20 temperature sensors connected via a OneWire bus.
// Functions:
// - setupDS18(): Initializes DS18B20 sensors and sets up sensor address assignments.
// - assignDS18Sensors(): Maps predefined sensor IDs to sensor addresses.
// - detectConnectedSensors(): Scans the OneWire bus to detect connected sensors.
// - readDS18(): Requests temperature readings from DS18B20 sensors and stores them.
// - getDS18SensorInfo(): Retrieves and optionally outputs information about connected DS18B20 sensors.

#include "ds18_module.h"

// Initialize OneWire bus and DallasTemperature library
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Arrays to store assigned and connected sensor addresses
DeviceAddress assignedAddresses[10];
DeviceAddress connectedAddresses[10];
int numAssignedSensors = 0;
int numConnectedSensors = 0;

// Function to set up DS18B20 sensors
void setupDS18() {
    sensors.begin();
    assignDS18Sensors();
    detectConnectedSensors();
    sensors.setWaitForConversion(true);  // Wait for temperature conversion
}

// Function to assign sensor IDs to addresses
void assignDS18Sensors() {
    const char* sensorIDs[] = {SENSOR6_ID, SENSOR7_ID, SENSOR8_ID, SENSOR9_ID, SENSOR10_ID, SENSOR11_ID, SENSOR12_ID, SENSOR13_ID, SENSOR14_ID, SENSOR15_ID};
    numAssignedSensors = sizeof(sensorIDs) / sizeof(sensorIDs[0]);
    for (int i = 0; i < numAssignedSensors && i < 10; i++) {
        if (strlen(sensorIDs[i]) > 0) {
            for (int j = 0; j < 8; j++) {
                unsigned int value;
                sscanf(&sensorIDs[i][j * 2], "%02X", &value);
                assignedAddresses[i][j] = static_cast<uint8_t>(value);
            }
        } else {
            memset(assignedAddresses[i], 0, sizeof(DeviceAddress));
        }
    }
}

// Function to detect connected sensors on the bus
void detectConnectedSensors() {
    numConnectedSensors = 0;
    sensors.begin();
    for (int i = 0; i < sensors.getDeviceCount() && i < 10; i++) {
        if (sensors.getAddress(connectedAddresses[i], i)) {
            numConnectedSensors++;
        } else {
            memset(connectedAddresses[i], 0, sizeof(DeviceAddress));
        }
    }
}

// Function to read temperatures from DS18B20 sensors
void readDS18(float temps[15]) {
    sensors.requestTemperatures();
    for (int i = 0; i < 10; i++) {
        if (i < numAssignedSensors && sensors.validAddress(assignedAddresses[i])) {
            temps[i + 5] = sensors.getTempC(assignedAddresses[i]);
            if (temps[i + 5] == DEVICE_DISCONNECTED_C) {
                temps[i + 5] = NAN;
            }
        } else {
            temps[i + 5] = NAN;
        }
    }
}

// Function to output DS18B20 sensor information
void getDS18SensorInfo() {
    for (int i = 0; i < numConnectedSensors; i++) {
        String sensorInfo = "Sensor " + String(i + 6) + " ID: ";
        for (uint8_t j = 0; j < 8; j++) {
            sensorInfo += String(connectedAddresses[i][j], HEX);
        }
        // Send the sensor information (uncomment if needed)
        // sendMessage(sensorInfo, "debug", 6);
    }
}
