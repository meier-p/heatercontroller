// Module: mcp41hv51_module.h
// Purpose: Declares the MCP41HV51 class and associated functions for controlling the digital potentiometer.
// Definitions:
// - MIN_TEMP, MAX_TEMP: Temperature range for mapping.
// - tempToMCPMap[]: Lookup table mapping temperatures to MCP values.
// Function Prototypes:
// - getMCPValue(): Helper function to map temperature to MCP value.
// Class:
// - MCP41HV51: Encapsulates methods to interact with the MCP41HV51 potentiometer.


#ifndef MCP41HV51_MODULE_H
#define MCP41HV51_MODULE_H

#include <Arduino.h>
#include <SPI.h>

// Temperature-to-MCP value mapping table
const int MIN_TEMP = -25;
const int MAX_TEMP = 50;
const uint8_t tempToMCPMap[MAX_TEMP - MIN_TEMP + 1] = {
    // Values mapping temperatures from -25°C to 50°C to MCP values
    // (This array should contain appropriate values for your application)
};

// Helper function to map temperature to MCP value
inline uint8_t getMCPValue(float temperature) {
    if (temperature < MIN_TEMP) {
        temperature = MIN_TEMP;
    }
    if (temperature > MAX_TEMP) {
        temperature = MAX_TEMP;
    }
    int index = round(temperature) - MIN_TEMP;
    return tempToMCPMap[index];
}

// Class representing the MCP41HV51 digital potentiometer
class MCP41HV51 {
public:
    MCP41HV51(uint8_t csPin);

    void begin();
    void setResistance(uint8_t value);      // Sets the resistance value (0-255)
    void setTemperature(float temperature); // Sets the MCP value based on temperature

private:
    uint8_t csPin; // Chip Select pin
    void sendCommand(uint8_t command, uint8_t value);
};

#endif
