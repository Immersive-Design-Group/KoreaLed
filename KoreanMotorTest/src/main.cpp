#include "Communication.hpp"
#include "Motor.hpp"
#include <Arduino.h>
#include <arduino-timer.h>

// create a timer that holds 1 tasks, with millisecond resolution,
// and a custom handler type of 'const char *
Timer< 1, micros, void* > t_timer;

bool StepperController( void* ) {
    if ( motorState == 1 ) {
        bool homing = true;
        for ( int i = 0; i < 5; i++ ) {
            homing &= cameras[ i ].moveHome();
            Serial.print( "Camera 1 " );
        }
        if ( homing ) {
            motorState = 2;
            for ( int i = 0; i < 5; i++ ) {
                cameras[ i ].setHoming( false );
            }
            digitalWrite( SLP, LOW );  // Sleep mode
            Serial.println("Sleeping:2");

        }
    }
    else if(motorState == 0) {
        bool isReached = true;
        for ( int i = 0; i < 5; i++ ) {
            isReached &= cameras[ i ].move();
            Serial.print( "Camera 0 " );
        }
        if ( isReached ) {
            motorState = 2;
            digitalWrite( SLP, LOW );  // Sleep mode
            Serial.println("Sleeping:0");


        }
    }
    return true;
}

void setup() {
    // put your setup code here, to run once:
    init();
}

void loop() {
    // put your main code here, to run repeatedly:
    SerialReceive();
    t_timer.tick();
}

void init() {
    Serial.begin( 115200 );
    Serial.println( "Init" );
    // Init the motors
    for ( int i = 0; i < 10; i++ ) {
        motors[ i ].begin( RPM );
    }
    pinMode( SLP, OUTPUT );
    digitalWrite( SLP, HIGH );  // Wake up mode
    // call the StepperController function
    t_timer.every( 20000, StepperController );
}