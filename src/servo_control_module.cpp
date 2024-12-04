// Module: servo_control_module.cpp
// Purpose: Controls servo motors that adjust air outlets for different heating zones.
// Functions:
// - setupServos(): Initializes servos and sets them to their starting positions.
// - setServoPosition(): Moves a servo to a specified angle based on a percentage (0-100% open).
// - getServoPosition(): Retrieves the current position (opening percentage) of a servo for a given zone.


#include "servo_control_module.h"

// Array to store servo configurations
ServoControl servos[MAX_SERVOS] = {
    {Servo(), 4, false, 0},
    {Servo(), 5, false, 0},
    {Servo(), 6, false, 0},
    {Servo(), 1, false, 0},
    {Servo(), 2, false, 0}
};

// Function to set up servos
void setupServos() {
    for (int i = 0; i < MAX_SERVOS; i++) {
        if (servos[i].pin != -1) {
            servos[i].servo.attach(servos[i].pin);
            servos[i].isAttached = true;
            servos[i].servo.write(0); // Set servo to 0° at start
            servos[i].currentAnglePercentage = 0;
            Serial.printf("Servo on pin %d set to starting position 0°.\n", servos[i].pin);
        }
    }
}

// Function to set the position of a servo based on zone index and angle percentage
void setServoPosition(int zoneIndex, int anglePercentage) {
    static bool isServoOperating = false;
    while (isServoOperating) {
        delay(10); // Wait until the current servo operation is complete
    }
    isServoOperating = true;
    // Check if the zone index is valid
    if (zoneIndex < 0 || zoneIndex >= NUM_ZONES) {
        Serial.println("Invalid zone index for servo positioning.");
        isServoOperating = false;
        return;
    }

    // Check if the servo for this zone is available
    int servoIndex = zones[zoneIndex].servoValve - 1;
    if (servoIndex < 0 || servoIndex >= MAX_SERVOS || !servos[servoIndex].isAttached) {
        Serial.printf("Servo for zone %d not available or not attached.\n", zoneIndex);
        isServoOperating = false;
        return;
    }

    // Calculate angle based on the specified opening percentage (0% to 90%)
    int angle = map(anglePercentage, 0, 100, 0, 90);

    // Move the servo slowly to the calculated angle
    int currentAngle = servos[servoIndex].servo.read();
    if (currentAngle < angle) {
        for (int pos = currentAngle; pos <= angle; pos++) {
            servos[servoIndex].servo.write(pos);
            delay(SERVO_SPEED_DELAY); // Slow upward movement
        }
    } else {
        for (int pos = currentAngle; pos >= angle; pos--) {
            servos[servoIndex].servo.write(pos);
            delay(SERVO_SPEED_DELAY); // Slow downward movement
        }
    }
    servos[servoIndex].currentAnglePercentage = anglePercentage;
    isServoOperating = false;
    Serial.printf("Servo in zone %d set to %d%% (angle: %d°, pin: %d).\n", zoneIndex, anglePercentage, angle, servos[servoIndex].pin);
}

// Function to get the current position (angle percentage) of a servo
int getServoPosition(int zoneIndex) {
    // Check if the zone index is valid
    if (zoneIndex < 0 || zoneIndex >= NUM_ZONES) {
        Serial.println("Invalid zone index for servo position query.");
        return -1;
    }

    // Check if the servo for this zone is available
    int servoIndex = zones[zoneIndex].servoValve - 1;
    if (servoIndex < 0 || servoIndex >= MAX_SERVOS || !servos[servoIndex].isAttached) {
        Serial.printf("Servo for zone %d not available or not attached.\n", zoneIndex);
        return -1;
    }

    return servos[servoIndex].currentAnglePercentage;
}
