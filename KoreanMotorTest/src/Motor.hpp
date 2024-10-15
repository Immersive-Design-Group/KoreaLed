#pragma once
#include "A4988.h"

// Motor state
int motorState = 1;  // 0: normal, 1: homing

// DIR, STEP, SW
const int motorPins[ 10 ][ 3 ] = { { 4, 5, 6 }, { 7, 15, 16 }, { 17, 18, 8 }, { 19, 20, 3 }, { 46, 9, 10 }, { 11, 12, 13 }, { 14, 21, 47 }, { 48, 45, 35 }, { 38, 37, 36 }, { 41, 40, 39 } };

// A4988 Stepper Motor Driver
const short STPS = 600;
const short RPM  = 60;
const int   SLP  = 42;

class DRV8428 : public A4988 {
private:
    short sw_pin;  // 添加开关引脚的成员变量

public:
    /*
     * Basic connection: only DIR, STEP are connected.
     * Microstepping controls should be hardwired.
     */
    DRV8428( short steps, short dir_pin, short step_pin, short sw_pin ) : A4988( steps, dir_pin, step_pin ), sw_pin( sw_pin ) {
        pinMode( sw_pin, INPUT );  // 初始化开关引脚
    }

    bool readSwitch() {
        if ( digitalRead( sw_pin ) ) {
            return false;
        }
        else {
            return true;
        }
    }
};

DRV8428 motors[ 10 ] = { DRV8428( STPS, motorPins[ 0 ][ 0 ], motorPins[ 0 ][ 1 ], motorPins[ 0 ][ 2 ] ), DRV8428( STPS, motorPins[ 1 ][ 0 ], motorPins[ 1 ][ 1 ], motorPins[ 1 ][ 2 ] ),
                         DRV8428( STPS, motorPins[ 2 ][ 0 ], motorPins[ 2 ][ 1 ], motorPins[ 2 ][ 2 ] ), DRV8428( STPS, motorPins[ 3 ][ 0 ], motorPins[ 3 ][ 1 ], motorPins[ 3 ][ 2 ] ),
                         DRV8428( STPS, motorPins[ 4 ][ 0 ], motorPins[ 4 ][ 1 ], motorPins[ 4 ][ 2 ] ), DRV8428( STPS, motorPins[ 5 ][ 0 ], motorPins[ 5 ][ 1 ], motorPins[ 5 ][ 2 ] ),
                         DRV8428( STPS, motorPins[ 6 ][ 0 ], motorPins[ 6 ][ 1 ], motorPins[ 6 ][ 2 ] ), DRV8428( STPS, motorPins[ 7 ][ 0 ], motorPins[ 7 ][ 1 ], motorPins[ 7 ][ 2 ] ),
                         DRV8428( STPS, motorPins[ 8 ][ 0 ], motorPins[ 8 ][ 1 ], motorPins[ 8 ][ 2 ] ), DRV8428( STPS, motorPins[ 9 ][ 0 ], motorPins[ 9 ][ 1 ], motorPins[ 9 ][ 2 ] ) };

class Camera {
private:
    DRV8428 motor1;
    DRV8428 motor2;

    int currentPitch = 0;
    int currentYaw   = 0;
    int targetPitch  = 0;
    int targetYaw    = 0;

public:
    Camera( DRV8428 motor1, DRV8428 motor2 ) : motor1( motor1 ), motor2( motor2 ) {}

    void setYaw( int yaw ) {
        targetYaw = yaw;
    }

    void setPitch( int pitch ) {
        targetPitch = pitch;
    }

    bool move() {
        // If the switch is pressed, reset the current position
        if ( !motor1.readSwitch() )
            currentPitch = 0;
        if ( !motor2.readSwitch() )
            currentPitch = 0;

        if ( currentPitch != targetPitch ) {
            if ( currentPitch < targetPitch ) {
                motor1.move( 1 );
                motor2.move( 1 );
                currentPitch++;
            }
            else {
                motor1.move( -1 );
                motor2.move( -1 );
                currentPitch--;
            }
            return false;
        }

        if ( currentYaw != targetYaw ) {
            if ( currentYaw < targetYaw ) {
                motor2.move( 1 );
                currentYaw++;
            }
            else {
                motor2.move( -1 );
                currentYaw--;
            }
            return false;
        }

        return true;
    }

    bool moveHome() {
        if ( !motor1.readSwitch() ) {
            motor1.move( -1 );
            motor2.move( -1 );
            return false;
        }
        else if ( !motor2.readSwitch() ) {
            motor2.move( -1 );
            return false;
        }
        else {
            currentPitch = 0;
            currentYaw   = 0;
            return true;
        }
    }
};

Camera cameras[ 5 ] = { Camera( motors[ 0 ], motors[ 1 ] ), Camera( motors[ 2 ], motors[ 3 ] ), Camera( motors[ 4 ], motors[ 5 ] ), Camera( motors[ 6 ], motors[ 7 ] ),
                        Camera( motors[ 8 ], motors[ 9 ] ) };