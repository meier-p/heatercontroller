// Module: data_module.cpp
// Purpose: Processes incoming MQTT data and updates the zone configurations accordingly.
// Functions:
// - processIncomingData(): Parses incoming MQTT messages to update the target temperatures for different zones based on the topic and message received.


#include "data_module.h"
#include "config.h"
#include "message_module.h"
#include "gpio_module.h"

// Function to process incoming MQTT data
void processIncomingData(const String &topic, const String &message) {
    sendMessage("Processing incoming data", "debug", 6);
    for (int i = 0; i < 10; i++) {
        // Construct the expected topic for the target temperature of the zone
        String expectedTopic = "N/" + String(MQTT_BASE_PATH) + "/" + String(zones[i].name) + "/target_temperature";
        if (topic == expectedTopic) {
            float newTarget = message.toFloat();
            // Debug: Show the parsed target temperature value
            sendMessage("Parsed target temperature value: " + String(newTarget), "debug", 6);

            if (!isnan(newTarget)) {
                // Update the target temperature for the zone
                zones[i].temperatureTarget = newTarget;
                // Debug: Confirm the assignment
                sendMessage("Updated target temperature for zone: " + String(zones[i].name) + " to " + String(zones[i].temperatureTarget), "debug", 6);
            } else {
                sendMessage("Received invalid target temperature value (NaN).", "debug", 6);
            }
        }
    }
}
