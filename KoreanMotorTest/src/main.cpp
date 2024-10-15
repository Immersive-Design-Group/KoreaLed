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
        }
        if ( homing ) {
            motorState = 0;
            for( int i = 0; i < 5; i++ ) {
                cameras[ i ].setHoming( false );
            }
            digitalWrite( SLP, LOW );  // Sleep mode
        }
    }
    else {
        bool isReached = true;
        for ( int i = 0; i < 5; i++ ) {
            isReached &= cameras[ i ].move();
        }
        if ( isReached ) {
            digitalWrite( SLP, LOW );  // Sleep mode
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
    t_timer.every( 8000, StepperController );
}