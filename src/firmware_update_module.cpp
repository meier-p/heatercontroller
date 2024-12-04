// Module: firmware_update_module.cpp
// Purpose: Handles checking for and performing firmware updates from a specified URL.
// Functions:
// - checkForFirmwareUpdate(): Checks for firmware updates, handles HTTP redirects, and performs the update if a new firmware is available.


#include "firmware_update_module.h"
#include "message_module.h" // For sending debug messages

// Function to check and perform firmware updates
void checkForFirmwareUpdate(const char* updateUrl, int redirectCount) {
    if (redirectCount > 5) { // Prevent infinite redirect loops
        sendMessage("Too many redirects. Aborting update.", "debug", 6);
        return;
    }

    WiFiClient client;
    HTTPClient http;

    sendMessage("Checking for firmware update...", "debug", 6);

    if (http.begin(client, updateUrl)) {
        http.addHeader("User-Agent", "ESP32-HeaterController/1.0"); // Required for GitHub URLs
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            int contentLength = http.getSize();

            if (contentLength > 0 && Update.begin(contentLength)) {
                sendMessage("Starting firmware update...", "debug", 6);

                WiFiClient* stream = http.getStreamPtr();
                size_t written = Update.writeStream(*stream);

                if (written == contentLength && Update.end()) {
                    sendMessage("Update successful. Restarting...", "debug", 6);
                    ESP.restart();
                } else {
                    sendMessage("Update failed: " + String(Update.getError()), "debug", 6);
                }
            } else {
                sendMessage("Not enough space for update or invalid content length.", "debug", 6);
            }
        } else if (httpCode == HTTP_CODE_FOUND || httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_SEE_OTHER) {
            // Handle HTTP 302, 301, 303 redirects
            String newUrl = http.header("Location");

            if (newUrl.length() > 0) {
                sendMessage("Redirected to: " + newUrl, "debug", 6);
                http.end(); // Close the current connection
                checkForFirmwareUpdate(newUrl.c_str(), redirectCount + 1); // Retry with the new URL
            } else {
                sendMessage("Redirection failed: Empty Location header. Attempting fallback.", "debug", 6);

                // Attempt a fallback solution for GitHub URLs
                if (String(updateUrl).startsWith("https://github.com/")) {
                    String fallbackUrl = String(updateUrl) + "?access=direct";
                    sendMessage("Fallback to: " + fallbackUrl, "debug", 6);
                    http.end();
                    checkForFirmwareUpdate(fallbackUrl.c_str(), redirectCount + 1);
                } else {
                    sendMessage("No fallback method available for this URL.", "debug", 6);
                }
            }
        } else {
            sendMessage("Failed to fetch firmware: HTTP Code " + String(httpCode), "debug", 6);
        }

        http.end();
    } else {
        sendMessage("Failed to connect to update server.", "debug", 6);
    }
}
