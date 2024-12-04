// Module: temperature_module.h
// Purpose: Declares functions and variables related to temperature management.
// Function Prototypes:
// - determineMainTemperature(): Computes the main temperature for control logic.
// External Variables:
// - mainTemperature: Holds the value of the main temperature used in the system.


#ifndef TEMPERATURE_MODULE_H
#define TEMPERATURE_MODULE_H

#include "config.h"
#include "zones_module.h"

// Function to determine the main temperature
float determineMainTemperature();

// Declare mainTemperature as external
extern float mainTemperature;

#endif // TEMPERATURE_MODULE_H
