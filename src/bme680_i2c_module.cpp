// Module: bme680_i2c.module.cpp
// Purpose: Manages initialization and data reading from BME680 environmental sensors over the I2C bus.
// Functions:
// - setupBME680(): Initializes BME680 sensors with specific settings for temperature, humidity, pressure, and gas measurements.
// - readBME680(): Reads temperature, humidity, pressure, and VOC (Volatile Organic Compounds) data from each BME680 sensor and stores the values in provided arrays.

#include "i2c.h"
#include <Wire.h>

#include "i2c.h"
#include <Wire.h>

// Array of BME680 sensor objects
Adafruit_BME680 bme680Sensors[BME680_SENSOR_COUNT] = {
    Adafruit_BME680(), Adafruit_BME680(), Adafruit_BME680(), Adafruit_BME680(), Adafruit_BME680()
};

// I2C addresses of the BME680 sensors (adjust according to your setup)
uint8_t bme680Addresses[BME680_SENSOR_COUNT] = {0x76, 0x77, 0x78, 0x79, 0x7A};

// Function to initialize the BME680 sensors
void setupBME680() {
    Wire.begin(); // Initialize I2C communication
    for (int i = 0; i < BME680_SENSOR_COUNT; i++) {
        // Try to initialize the sensor at the given address
        if (!bme680Sensors[i].begin(bme680Addresses[i])) {
            sendMessage("Could not find BME680 Sensor " + String(i) + "!", "debug", 6);
        } else {
            // Configure oversampling settings
            bme680Sensors[i].setTemperatureOversampling(BME680_OS_8X);
            bme680Sensors[i].setHumidityOversampling(BME680_OS_2X);
            bme680Sensors[i].setPressureOversampling(BME680_OS_4X);
            bme680Sensors[i].setIIRFilterSize(BME680_FILTER_SIZE_3);
            bme680Sensors[i].setGasHeater(320, 150); // Set gas heater to 320°C for 150 ms

            // Send the sensor ID as an MQTT message
            String sensorID = "BME680 Sensor " + String(i) + " Address: " + String(bme680Addresses[i], HEX);
            sendMessage(sensorID, "sensor", 1); // Send the message with MQTT priority 1
        }
    }
}

// Function to read data from the BME680 sensors
void readBME680(float sensors[], float hums[], float pressures[], float vocs[]) {
    for (int i = 0; i < BME680_SENSOR_COUNT; i++) {
        if (bme680Sensors[i].performReading()) {
            // Store temperature value
            sensors[15 + i] = bme680Sensors[i].temperature;
            // Store humidity value
            hums[15 + i] = bme680Sensors[i].humidity;
            // Store pressure value in hPa, rounded to one decimal place
            pressures[15 + i] = round(bme680Sensors[i].pressure / 100.0 * 10) / 10.0;
            // Store VOC value in kΩ, rounded to the nearest whole number
            vocs[15 + i] = round(bme680Sensors[i].gas_resistance / 1000.0);

            // Optional: Send the read values as an MQTT message
            // sendMessage("BME680 Sensor " + String(i) + " read: Temp=" + String(sensors[15 + i]) + ", Hum=" + String(hums[15 + i]) + ", Pressure=" + String(pressures[15 + i]) + ", VOC=" + String(vocs[15 + i]) + ", ID=" + String(bme680Addresses[i], HEX), "sensor", 1);
        }
    }
}
