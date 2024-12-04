// Module: dht_module.h
// Purpose: Declares functions and constants related to DHT sensors.
// Definitions:
// - DHT sensor pins and type definitions.
// Function Prototypes:
// - setupDHT(): Initializes the DHT sensors.
// - readDHT(): Reads data from the DHT sensors.

#ifndef DHT_MODULE_H
#define DHT_MODULE_H

#include <DHT.h>
#include "config.h"

// Define the pins for the DHT sensors
#define DHTPIN_1 7  
#define DHTPIN_2 15
#define DHTPIN_3 16
#define DHTPIN_4 17
#define DHTPIN_5 18
#define DHTTYPE DHT22  // Sensor type

// Function prototypes
void setupDHT();
void readDHT(float &temp1, float &hum1, float &temp2, float &hum2, float &temp3, float &hum3, float &temp4, float &hum4, float &temp5, float &hum5);

#endif
