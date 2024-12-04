// Module: ds18_module.h
// Purpose: Declares functions and variables related to DS18B20 sensors.
// Definitions:
// - ONE_WIRE_BUS: Pin where DS18B20 sensors are connected.
// - SENSOR IDs: Predefined IDs for identifying specific DS18B20 sensors.
// Function Prototypes:
// - setupDS18()
// - readDS18()
// - getDS18SensorInfo()
// - assignDS18Sensors()
// - detectConnectedSensors()
// External Variables:
// - numAssignedSensors, assignedAddresses[], numConnectedSensors, connectedAddresses[]


#ifndef DS18_MODULE_H
#define DS18_MODULE_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.h"

// Define the pin for the OneWire bus
#define ONE_WIRE_BUS 21 // Pin where DS18B20 sensors are connected

// Define IDs of the DS18B20 sensors
#define SENSOR6_ID "28FF641F7C4EE140"
#define SENSOR7_ID "28FF641F7C64B821"
#define SENSOR8_ID "28FF641F7C44D2FB"
#define SENSOR9_ID "28FF641F7FB8C5D5"
#define SENSOR10_ID "28FF641F7FDA9246"
#define SENSOR11_ID ""
#define SENSOR12_ID ""
#define SENSOR13_ID ""
#define SENSOR14_ID ""
#define SENSOR15_ID ""

// Function declarations
void setupDS18();
void readDS18(float temps[15]);
void getDS18SensorInfo();
void assignDS18Sensors();
void detectConnectedSensors();

// External variable declarations
extern int numAssignedSensors;
extern DeviceAddress assignedAddresses[10];
extern int numConnectedSensors;
extern DeviceAddress connectedAddresses[10];

#endif
