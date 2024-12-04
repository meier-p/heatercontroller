// Module: gpio_module.cpp
// Purpose: Manages General Purpose Input/Output (GPIO) pins related to the heater's status and control.
// Functions:
// - setupGPIO(): Configures GPIO pins for reading heater status and controlling the heater.
// - readHeaterStatus(): Reads the current status of the heater from a GPIO pin.
// - toggleHeater(): Toggles the heater on or off based on the specified control mode.


#include "gpio_module.h"

// Global variables for heater status and control mode
bool heaterStatus = false;
HeaterControlMode heaterControlMode = TOGGLE;

// Function to set up GPIO pins
void setupGPIO() {
    pinMode(HEATER_STATUS_PIN, INPUT);    // Set heater status pin as input
    pinMode(HEATER_TOGGLE_PIN, OUTPUT);   // Set heater toggle pin as output
    digitalWrite(HEATER_TOGGLE_PIN, LOW); // Initialize toggle pin to LOW
}

// Function to read the current heater status
void readHeaterStatus() {
    bool currentStatus = digitalRead(HEATER_STATUS_PIN);
    if (currentStatus != heaterStatus) {
        heaterStatus = currentStatus; // Update global heater status
    }
}

// Function to control the heater (turn on/off)
void toggleHeater(bool state) {
    if (heaterControlMode == SWITCH) {
        // Directly set the output state
        digitalWrite(HEATER_TOGGLE_PIN, state ? HIGH : LOW);
    } else if (heaterControlMode == TOGGLE) {
        // Toggle the heater state
        digitalWrite(HEATER_TOGGLE_PIN, HIGH);
        delay(500);  // Hold the toggle for 0.5 seconds
        digitalWrite(HEATER_TOGGLE_PIN, LOW);
    }
}
