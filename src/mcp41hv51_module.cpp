// Module: mcp41hv51_module.cpp
// Purpose: Manages the MCP41HV51 digital potentiometer used for setting temperature values.
// Class Methods:
// - MCP41HV51::begin(): Initializes SPI communication and sets up the chip select (CS) pin.
// - MCP41HV51::setResistance(): Sets the resistance value of the potentiometer.
// - MCP41HV51::setTemperature(): Converts a temperature value to a resistance value and sets it.


#include "mcp41hv51_module.h"

// Constructor for MCP41HV51 class
MCP41HV51::MCP41HV51(uint8_t csPin) : csPin(csPin) {}

// Initialize the MCP41HV51 potentiometer
void MCP41HV51::begin() {
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH); // Deactivate chip select
    SPI.begin();
}

// Set the resistance value of the potentiometer (0-255)
void MCP41HV51::setResistance(uint8_t value) {
    if (value > 255) {
        value = 255; // Limit to 255
    }
    sendCommand(0x00, value); // 0x00 is the command to write potentiometer data
    Serial.println("Resistance set to: " + String(value));
}

// Send a command to the MCP41HV51 via SPI
void MCP41HV51::sendCommand(uint8_t command, uint8_t value) {
    digitalWrite(csPin, LOW); // Select the chip
    SPI.transfer(command);
    SPI.transfer(value);
    digitalWrite(csPin, HIGH); // Deselect the chip
}

// Set the temperature by mapping it to a resistance value
void MCP41HV51::setTemperature(float temperature) {
    uint8_t mcpValue = getMCPValue(temperature);
    setResistance(mcpValue);
}
