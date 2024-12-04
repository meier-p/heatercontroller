// Module: data_module.h
// Purpose: Declares functions for processing incoming data, particularly MQTT messages, and updating the system state accordingly.
// Function Prototypes:
// - processIncomingData(const String &topic, const String &message): Processes incoming MQTT messages to update zone configurations such as target temperatures.

#ifndef DATA_MODULE_H
#define DATA_MODULE_H

#include "config.h"
#include <Arduino.h>

// Function prototype for processing incoming MQTT data
void processIncomingData(const String &topic, const String &message);

#endif // DATA_MODULE_H
