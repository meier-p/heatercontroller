// Module: heater_automation_module.cpp
// Purpose: Contains automation logic for controlling the heater and servo valves based on zone temperatures and targets.
// Functions:
// - isAutomationActive(): Returns the status of the automation system.
// - controlExternalHeater(): Controls the external heater (turns it on or off).
// - controlHeaterBasedOnZones(): Determines whether to activate or deactivate the heater based on temperature readings and hysteresis thresholds.
// - controlServoValvesBasedOnZones(): Adjusts servo-controlled valves in each zone based on temperature targets and whether the heater is active.


#include "heater_automation_module.h"
#include "gpio_module.h"
#include "temperature_module.h"
#include "servo_control_module.h"
#include <Arduino.h>

// External declarations for heater status and zones
extern bool heaterStatus;
extern Zone zones[NUM_ZONES];

// Global variables for automation status and valve mode
bool automationActive = false;          // Indicates if automation is active
bool valveModeProportional = false;     // Default to proportional mode (true for proportional, false for on/off)

// Function to check if automation is active
bool isAutomationActive() {
    return automationActive;
}

// Function to control the external heater (turn on/off)
void controlExternalHeater(bool turnOn) {
    if (turnOn && !heaterStatus) {
        // Turn on the heater if it's not already on
        toggleHeater(true);
    } else if (!turnOn && heaterStatus) {
        // Turn off the heater if it's currently on
        toggleHeater(false);
    }
}

// Function to control the heater based on zone temperatures
void controlHeaterBasedOnZones() {
    bool shouldTurnOnHeater = false;
    bool shouldTurnOffHeater = true;

    for (int i = 0; i < NUM_ZONES; i++) {
        // Check if the zone is valid and has temperature data
        if (strlen(zones[i].name) > 0 && !isnan(zones[i].temperature) && !isnan(zones[i].temperatureTarget)) {
            float lowerThreshold = zones[i].temperatureTarget - HYSTERESIS_UNDER;
            float upperThreshold = zones[i].temperatureTarget + HYSTERESIS_OVER;

            if (heaterStatus) {
                // If the heater is on, check if any zone is below the upper threshold
                if (zones[i].temperature < upperThreshold) {
                    shouldTurnOffHeater = false;
                }
            } else {
                // If the heater is off, check if any zone is below the lower threshold
                if (zones[i].temperature < lowerThreshold) {
                    shouldTurnOnHeater = true;
                }
            }
        }
    }

    // Decide whether to turn the heater on or off based on the above checks and automation status
    if (heaterStatus && shouldTurnOffHeater && automationActive) {
        controlExternalHeater(false);
    } else if (!heaterStatus && shouldTurnOnHeater && automationActive) {
        controlExternalHeater(true);
    }
}

// Function to control servo valves based on zone temperatures
void controlServoValvesBasedOnZones() {
    // If the heater is off, open all servos fully
    if (!heaterStatus) {
        for (int i = 0; i < NUM_ZONES; i++) {
            if (strlen(zones[i].name) > 0 && zones[i].servoValve > 0) {
                setServoPosition(i, 100); // Fully open
            }
        }
        return;
    }
    for (int i = 0; i < NUM_ZONES; i++) {
        if (strlen(zones[i].name) > 0 && zones[i].servoValve > 0) {
            if (!isnan(zones[i].temperature) && !isnan(zones[i].temperatureTarget)) {
                int anglePercentage = 0;
                float lowerThreshold = zones[i].temperatureTarget - HYSTERESIS_UNDER;
                float upperThreshold = zones[i].temperatureTarget + HYSTERESIS_OVER;

                if (valveModeProportional) {
                    // Proportional mode: Open the valve proportionally to the difference between current and target temperature
                    if (zones[i].temperature < zones[i].temperatureTarget) {
                        float difference = zones[i].temperatureTarget - zones[i].temperature;
                        anglePercentage = min(100, (int)(difference * 10)); // Max 100%
                    }
                } else {
                    // On/Off mode: Open or close the valve based on hysteresis thresholds
                    if (zones[i].temperature < lowerThreshold) {
                        anglePercentage = 100; // Fully open
                    } else if (zones[i].temperature > upperThreshold) {
                        anglePercentage = 0; // Fully closed
                    }
                }

                setServoPosition(i, anglePercentage);
            }
        }
    }
}
