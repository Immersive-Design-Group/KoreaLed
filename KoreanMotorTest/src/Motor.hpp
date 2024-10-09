#pragma once
#include "A4988.h"

// DIR, STEP, SW
const int motorPins[ 10 ][ 3 ] = { { 4, 5, 6 }, { 7, 15, 16 }, { 17, 18, 8 }, { 19, 20, 3 }, { 46, 9, 10 }, { 11, 12, 13 }, { 14, 21, 47 }, { 48, 45, 35 }, { 38, 37, 36 }, { 41, 40, 39 } };

// A4988 Stepper Motor Driver
const short STPS = 600;
const short RPM  = 60;
const int   SLP  = 42;

class DRV8428 : public A4988 {
private:
    short sw_pin;  // 添加开关引脚的成员变量
    int   currentStep = 0;
    int   targetStep  = 0;

public:
    /*
     * Basic connection: only DIR, STEP are connected.
     * Microstepping controls should be hardwired.
     */
    DRV8428( short steps, short dir_pin, short step_pin, short sw_pin ) : A4988( steps, dir_pin, step_pin ), sw_pin( sw_pin ) {
        pinMode( sw_pin, INPUT );  // 初始化开关引脚
    }

    // 重写必要的方法以适应DRV8428的特性
    short setMicrostep( short microsteps ) override {
        return A4988::setMicrostep( microsteps );
    }

    int readSwitch() {
        if ( digitalRead( sw_pin ) ) {
            setCurrentStep( 0 );
            return 1;
        }
        else {
            return 0;
        }
    }

    int getCurrentStep() {
        return currentStep;
    }

    void setCurrentStep( int step ) {
        currentStep = step;
    }

    int getTargetStep() {
        return targetStep;
    }

    void setTargetStep( int step ) {
        targetStep = step;
    }

    int moveTo( void ) {
        readSwitch();
        if ( currentStep < targetStep ) {
            move( 1 );
            currentStep++;
            return 0;
        }
        else if ( currentStep > targetStep ) {
            move( -1 );
            currentStep--;
            return 0;
        }
        else {
            return 1;
        }
    }

protected:
    // 重写获取微步表的方法
    const uint8_t* getMicrostepTable() override {
        return A4988::getMicrostepTable();
    }

    size_t getMicrostepTableSize() override {
        return A4988::getMicrostepTableSize();
    }

    short getMaxMicrostep() override {
        return A4988::getMaxMicrostep();
    }
};

DRV8428 motors[ 10 ] = { DRV8428( STPS, motorPins[ 0 ][ 0 ], motorPins[ 0 ][ 1 ], motorPins[ 0 ][ 2 ] ), DRV8428( STPS, motorPins[ 1 ][ 0 ], motorPins[ 1 ][ 1 ], motorPins[ 1 ][ 2 ] ),
                         DRV8428( STPS, motorPins[ 2 ][ 0 ], motorPins[ 2 ][ 1 ], motorPins[ 2 ][ 2 ] ), DRV8428( STPS, motorPins[ 3 ][ 0 ], motorPins[ 3 ][ 1 ], motorPins[ 3 ][ 2 ] ),
                         DRV8428( STPS, motorPins[ 4 ][ 0 ], motorPins[ 4 ][ 1 ], motorPins[ 4 ][ 2 ] ), DRV8428( STPS, motorPins[ 5 ][ 0 ], motorPins[ 5 ][ 1 ], motorPins[ 5 ][ 2 ] ),
                         DRV8428( STPS, motorPins[ 6 ][ 0 ], motorPins[ 6 ][ 1 ], motorPins[ 6 ][ 2 ] ), DRV8428( STPS, motorPins[ 7 ][ 0 ], motorPins[ 7 ][ 1 ], motorPins[ 7 ][ 2 ] ),
                         DRV8428( STPS, motorPins[ 8 ][ 0 ], motorPins[ 8 ][ 1 ], motorPins[ 8 ][ 2 ] ), DRV8428( STPS, motorPins[ 9 ][ 0 ], motorPins[ 9 ][ 1 ], motorPins[ 9 ][ 2 ] ) };