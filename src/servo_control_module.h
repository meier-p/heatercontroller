// Module: servo_control_module.h
// Purpose: Declares functions and structures for controlling servo motors.
// Definitions:
// - MAX_SERVOS: Maximum number of servos supported.
// - SERVO_SPEED_DELAY: Delay between servo movements for smooth operation.
// Structures:
// - ServoControl: Contains information about a servo, including its pin, attachment status, and current angle.
// Function Prototypes:
// - setupServos()
// - setServoPosition()
// - getServoPosition()


#ifndef SERVO_CONTROL_MODULE_H
#define SERVO_CONTROL_MODULE_H

#include "config.h"
#include <Arduino.h>
#include <ESP32Servo.h> // Use the ESP32-specific Servo library

#define MAX_SERVOS 5          // Maximum number of connected servos
#define SERVO_SPEED_DELAY 5   // Delay for smooth servo movement (in ms)

// Structure to hold servo control information
struct ServoControl {
    Servo servo;
    int pin;
    bool isAttached;
    int currentAnglePercentage; // Stores the current opening percentage
};

// Function prototypes
void setupServos();
void setServoPosition(int zoneIndex, int anglePercentage);
int getServoPosition(int zoneIndex);

#endif // SERVO_CONTROL_MODULE_H
