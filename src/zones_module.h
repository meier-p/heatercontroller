// Module: zones_module.h
// Purpose: Declares functions related to zone management.
// Function Prototypes:
// - assignSensorValues(): Assigns sensor data to zones.


#ifndef ZONES_MODULE_H
#define ZONES_MODULE_H

#include "config.h"

// Function prototype for assigning sensor values to zones
void assignSensorValues(float sensors[NUM_ZONES], float hums[NUM_ZONES], float pressures[NUM_ZONES], float vocs[NUM_ZONES]);

#endif // ZONES_MODULE_H
