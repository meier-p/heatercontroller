// Module: gpio_module.h
// Purpose: Declares GPIO-related functions and variables for heater control.
// Definitions:
// - HEATER_STATUS_PIN, HEATER_TOGGLE_PIN: GPIO pins for heater status and control.
// Enumerations:
// - HeaterControlMode: Defines control modes such as TOGGLE and SWITCH.
// External Variables:
// - heaterStatus: Indicates the current status of the heater.
// - heaterControlMode: Defines the control mode for the heater.
// Function Prototypes:
// - setupGPIO()
// - readHeaterStatus()
// - toggleHeater()


#ifndef GPIO_MODULE_H
#define GPIO_MODULE_H

#include <Arduino.h>

// GPIO pin definitions
#define HEATER_STATUS_PIN 38 // Pin to read heater status
#define HEATER_TOGGLE_PIN 42 // Pin to control heater toggle

// Enumeration for heater control modes
enum HeaterControlMode {
    TOGGLE,
    SWITCH
};

// External variables for heater status and control mode
extern bool heaterStatus;
extern HeaterControlMode heaterControlMode;

// Function prototypes
void setupGPIO();
void readHeaterStatus();
void toggleHeater(bool state);

#endif
