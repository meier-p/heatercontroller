// Module: i2c.h
// Purpose: Declares functions and variables for I2C communication with BME680 sensors.
// Definitions:
// - BME680_SENSOR_COUNT: Number of BME680 sensors.
// Structures:
// - BME680Data: Stores sensor readings.
// External Variables:
// - sensors[], hums[], pressures[], vocs[]: Arrays to store sensor data.
// - bme680Sensors[], bme680Addresses[]: Arrays of sensor objects and their I2C addresses.
// Function Prototypes:
// - setupBME680()
// - readBME680()


#ifndef I2C_H
#define I2C_H

#include <Adafruit_BME680.h>
#include "message_module.h"
#include "config.h"

// Define the number of BME680 sensors
#define BME680_SENSOR_COUNT 5

// Structure to store values from a BME680 sensor
struct BME680Data {
    float temperature;
    float humidity;
    float pressure;
    float voc;
};

// External declarations of sensor arrays
extern float sensors[20];
extern float hums[20];
extern float pressures[20];
extern float vocs[20];

// External declarations of BME680 sensors and their I2C addresses
extern Adafruit_BME680 bme680Sensors[BME680_SENSOR_COUNT];
extern uint8_t bme680Addresses[BME680_SENSOR_COUNT];

// Function prototypes for initializing and reading BME680 sensors
void setupBME680();
void readBME680(float sensors[], float hums[], float pressures[], float vocs[]);

#endif
