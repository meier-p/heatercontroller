#include "config.h"
#include <Arduino.h>

// Initialize the zones array with default values
Zone zones[NUM_ZONES] = {
    {"Cabin", NAN, NAN, 4.0, NAN, 1, NAN, NAN},
    {"Bath", NAN, NAN, 4.0, NAN, 2, NAN, NAN},
    {"Plicht", NAN, NAN, 4.0, NAN, 3, NAN, NAN},
    {"Air Inlet", NAN, NAN, 20.0, NAN, 4, NAN, NAN},
    {"Underfloor", NAN, NAN, 23.0, NAN, 5, NAN, NAN},
    {"Bilge", NAN, NAN, 18.0, NAN, 6, NAN, NAN},
    {"Reserve", NAN, NAN, 20.0, NAN, 7, NAN, NAN},
    {"Airtronic", NAN, NAN, 19.0, NAN, 8, NAN, NAN},
    {"", NAN, NAN, NAN, NAN, -1, NAN, NAN},
    {"", NAN, NAN, NAN, NAN, -1, NAN, NAN}
};

// Function to assign sensor values to zones
void assignSensorValues(float sensors[20], float hums[20], float pressures[20], float vocs[20]) {
    zones[0].temperature = sensors[16];
    zones[0].humidity = hums[16];
    zones[0].temperatureValve = sensors[8];
    zones[0].pressure = pressures[16];
    zones[0].voc = vocs[16];

    zones[1].temperature = sensors[4];
    zones[1].humidity = hums[4];
    zones[1].pressure = pressures[4];
    zones[1].voc = vocs[4];

    zones[2].temperature = sensors[5];

    // Additional assignments can be added here as needed

    /*
    // Example of commented out assignments
    zones[3].temperature = sensors[3];
    zones[3].humidity = hums[3];
    zones[3].temperatureValve = sensors[11];
    zones[3].pressure = pressures[3];
    zones[3].voc = vocs[3];

    // Continue for other zones...
    */

    // Optional: Print zone information for debugging
    /*
    for (int i = 0; i < NUM_ZONES; i++) {
        Serial.printf("Zone %d (%s): Temp=%.2f, Hum=%.2f, Press=%.2f, VOC=%.2f\n",
                      i, zones[i].name, zones[i].temperature, zones[i].humidity, zones[i].pressure, zones[i].voc);
    }
    */
}
