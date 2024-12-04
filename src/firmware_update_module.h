// Module: firmware_update_module.h
// Purpose: Declares the function for firmware updates.
// Function Prototypes:
// - checkForFirmwareUpdate(): Initiates the firmware update process.


#ifndef FIRMWARE_UPDATE_MODULE_H
#define FIRMWARE_UPDATE_MODULE_H

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Update.h>

// Function to check for firmware updates
void checkForFirmwareUpdate(const char* updateUrl, int redirectCount = 0);

#endif
