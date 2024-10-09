#include "Communication.hpp"
#include "Motor.hpp"
#include <Arduino.h>
#include <arduino-timer.h>

// create a timer that holds 1 tasks, with millisecond resolution,
// and a custom handler type of 'const char *
Timer< 1, micros, void* > t_timer;

bool StepperController( void* ) {
    short reachCount = 0;
    for ( int i = 0; i < 10; i++ ) {
        if ( motors[ i ].moveTo() ) {
            reachCount++;
        }
    }
    if ( reachCount == 10 ) {
		digitalWrite( SLP, LOW );  // Sleep mode
		Serial.println( "Sleep" );
    }
    return true;
}

void setup() {
    // put your setup code here, to run once:
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
    digitalWrite( SLP, LOW );  // Slepp mode
    // call the StepperController function
    t_timer.every( 2000, StepperController );
}