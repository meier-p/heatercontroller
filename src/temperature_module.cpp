// Module: temperature_module.cpp
// Purpose: Determines the main temperature used for control logic and updates the digital potentiometer accordingly.
// Functions:
// - determineMainTemperature(): Calculates the primary temperature to be used for heater control, typically the lowest temperature below target among all zones.


#include "temperature_module.h"
#include "message_module.h"
#include "mcp41hv51_module.h"
#include <Arduino.h>

// External instance of the MCP41HV51 module
extern MCP41HV51 mcp41hv51;
// External zones array
extern Zone zones[NUM_ZONES];

// Define the mainTemperature variable
float mainTemperature = NAN;

// Function to determine the main temperature used for control logic
float determineMainTemperature() {
    float selectedTemperature = NAN;
    int selectedZoneIndex = -1;

    // Find the coldest zone below its target temperature
    for (int i = 0; i < NUM_ZONES; i++) {
        if (strlen(zones[i].name) > 0 && !isnan(zones[i].temperature) && !isnan(zones[i].temperatureTarget)) {
            if (zones[i].temperature < zones[i].temperatureTarget) {
                if (isnan(selectedTemperature) || zones[i].temperature < selectedTemperature) {
                    selectedTemperature = zones[i].temperature;
                    selectedZoneIndex = i;
                }
            }
        }
    }

    // Fallback to zone[0] if no zone is below its target temperature
    if (isnan(selectedTemperature)) {
        if (!isnan(zones[0].temperature)) {
            selectedTemperature = zones[0].temperature;
            selectedZoneIndex = 0;
        }
    }

    // Update the main temperature only if it has changed
    if (!isnan(selectedTemperature) && selectedZoneIndex != -1) {
        float roundedTemperature = round(selectedTemperature); // Round to whole numbers
        if (isnan(mainTemperature) || mainTemperature != roundedTemperature) {
            Serial.printf("mainTemperature changes from %.2f to %.2f (Zone: %s)\n",
                          mainTemperature, roundedTemperature, zones[selectedZoneIndex].name);
            mainTemperature = roundedTemperature;

            // Set the MCP value based on the rounded main temperature
            // mcp41hv51.setTemperature(mainTemperature);
        } else {
            Serial.printf("mainTemperature remains the same: %.2f\n", mainTemperature);
        }
    }

    return mainTemperature;
}
