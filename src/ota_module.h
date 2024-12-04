// Module: ota_module.h
// Purpose: Declares functions for OTA updates and WiFi setup.
// Function Prototypes:
// - setupWiFi()
// - setupOTA()


#ifndef OTA_MODULE_H
#define OTA_MODULE_H

#include <ArduinoOTA.h>
#include <WiFi.h>
#include "config.h"

// Function prototypes
void setupWiFi();
void setupOTA();

#endif
