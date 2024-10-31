#pragma once
#include "A4988.h"

// Motor ratios
const float motorRatio1 = 10.0f / 6.0f;  // Ratio for motor 1
const float motorRatio2 = 10.0f / 6.0f * 19.0f / 2.0f;  // Ratio for motor 2

// Motor states: 0 means normal, 1 means homing, 2 means idle
int motorState = 0;  

// Motor pin configuration: DIR, STEP, SW
const int motorPins1[5][3] = { { 17, 18, 8 }, { 38, 37, 36 }, { 4, 5, 6 }, { 48, 45, 35 }, { 46, 9, 10 } };
const int motorPins2[5][3] = { { 19, 20, 3 }, { 41, 40, 39 }, { 7, 15, 16 }, { 14, 21, 47 }, { 11, 12, 13 } };

// A4988 stepper motor driving parameters
const short STPS = 1600;  // Steps per rotation
const short RPM  = 60;   // Speed in revolutions per minute
const int   SLP  = 42;   // Sleep pin

class DRV8428 : public A4988 {
private:
    short sw_pin;  // Member variable for the switch pin

public:
    /*
     * Basic connection: Only DIR and STEP pins are connected.
     * Micro-stepping control should be hard-wired.
     */
    // Constructor to initialize motor parameters and switch pin
    DRV8428(short steps, short dir_pin, short step_pin, short sw_pin) : A4988(steps, dir_pin, step_pin), sw_pin(sw_pin) {
        pinMode(sw_pin, INPUT);  // Initialize the switch pin
    }

    // Read switch state, returns true if pressed
    bool readSwitch() {
        return digitalRead(sw_pin) == LOW;  // Returns true when switch is pressed
    }
};

// Initialize multiple stepper motors
DRV8428 motors[10] = { 
    DRV8428(STPS, motorPins1[0][0], motorPins1[0][1], motorPins1[0][2]), 
    DRV8428(STPS, motorPins2[0][0], motorPins2[0][1], motorPins2[0][2]),
    DRV8428(STPS, motorPins1[1][0], motorPins1[1][1], motorPins1[1][2]), 
    DRV8428(STPS, motorPins2[1][0], motorPins2[1][1], motorPins2[1][2]),
    DRV8428(STPS, motorPins1[2][0], motorPins1[2][1], motorPins1[2][2]), 
    DRV8428(STPS, motorPins2[2][0], motorPins2[2][1], motorPins2[2][2]),
    DRV8428(STPS, motorPins1[3][0], motorPins1[3][1], motorPins1[3][2]), 
    DRV8428(STPS, motorPins2[3][0], motorPins2[3][1], motorPins2[3][2]),
    DRV8428(STPS, motorPins1[4][0], motorPins1[4][1], motorPins1[4][2]), 
    DRV8428(STPS, motorPins2[4][0], motorPins2[4][1], motorPins2[4][2]) 
};

class Camera {
private:
    DRV8428 motor1;  // Motor 1 for the camera
    DRV8428 motor2;  // Motor 2 for the camera

    int currentPitch = 0;  // Current pitch angle
    int currentYaw   = 0;  // Current yaw angle
    int targetPitch  = 0;  // Target pitch angle
    int targetYaw    = 0;  // Target yaw angle

    bool isHoming = false;  // Indicates if currently homing

public:
    // Constructor to initialize camera motors
    Camera(DRV8428 motor1, DRV8428 motor2) : motor1(motor1), motor2(motor2) {}

    // Set the target yaw angle
    void setYaw(int yaw) {
        targetYaw = yaw;
    }

    // Set the target pitch angle
    void setPitch(int pitch) {
        targetPitch = pitch;
    }

    // Move the camera to the target angles
    bool move() {
        // If the switch is pressed, reset the current position
        if (motor1.readSwitch()) {
            currentPitch = 0;
            // Serial.println("press1");
        }
            
        if (motor2.readSwitch()) {
            currentYaw = 0;
            // Serial.println("press2");
        }

        // Move to target pitch angle
        if (currentPitch != targetPitch) {
            if (currentPitch < targetPitch) {
                motor1.move(-1);  // Move up
                motor2.move(-1);  // Move synchronously
                currentPitch++;
                // Serial.println("up");
            } else {
                motor1.move(1);  // Move down
                motor2.move(1);  // Move synchronously
                currentPitch--;
                // Serial.println("down");
            }
            return false;  // Not yet reached target
        }

        // Move to target yaw angle
        if (currentYaw != targetYaw) {
            if (currentYaw < targetYaw) {
                motor2.move(1);  // Move right
                currentYaw++;
            } else {
                motor2.move(-1);  // Move left
                currentYaw--;
            }
            return false;  // Not yet reached target
        }

        return true;  // Reached target
    }

    // Homing function
    bool moveHome() {
        if (isHoming) {
            return true;  // Already homing
        }
        if (!motor1.readSwitch()) {
            motor1.move(1);  // Move motor 1 up
            motor2.move(1);  // Move motor 2 up
            return false;  // Continue moving
        } else if (!motor2.readSwitch()) {
            motor2.move(-1);  // Move motor 2 down
            return false;  // Continue moving
        } else {
            // Both reached the bottom switch, reset position
            currentPitch = 0;
            currentYaw   = 0;
            targetPitch  = 0;
            targetYaw    = 0;
            isHoming     = true;  // Set to homing state
            return true;  // Complete homing
        }
    }

    // Set the homing state
    void setHoming(bool homing) {
        isHoming = homing;
    }
};

// Initialize multiple cameras
Camera cameras[5] = { 
    Camera(motors[0], motors[1]), 
    Camera(motors[2], motors[3]), 
    Camera(motors[4], motors[5]), 
    Camera(motors[6], motors[7]),
    Camera(motors[8], motors[9]) 
};
