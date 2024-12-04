// Module: heater_automation_module.h
// Purpose: Declares functions and variables for heater automation control.
// External Variables:
// - automationActive: Indicates whether automation is active.
// - valveModeProportional: Determines if valve control is proportional or on/off.
// Function Prototypes:
// - isAutomationActive()
// - controlExternalHeater()
// - controlHeaterBasedOnZones()
// - controlServoValvesBasedOnZones()


#ifndef HEATER_AUTOMATION_MODULE_H
#define HEATER_AUTOMATION_MODULE_H

#include "config.h"

// External variables indicating automation status and valve mode
extern bool automationActive;
extern bool valveModeProportional; // True: proportional control, False: on/off control

// Function prototypes
bool isAutomationActive();
void controlExternalHeater(bool turnOn);
void controlHeaterBasedOnZones();
void controlServoValvesBasedOnZones();

#endif // HEATER_AUTOMATION_MODULE_H
