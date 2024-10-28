#pragma once

#include "Motor.hpp"
#include <ArduinoJson.h>

static int Command[ 3 ];
String     buff = "";
// {"device":0,"state":0,"target":[100, 200, 300, 400, 500]}
// {"device":1,"state":0,"target":[100, 200, 300, 400, 500]}

// JSON object
JsonDocument jb;

void SerialReceive() {
    if ( Serial.available() ) {
        char inChar;
        while ( inChar != '\n' ) {
            while ( !Serial.available() )
                ;
            inChar = ( char )Serial.read();
            if ( inChar == '\n' ) {
                DeserializationError err = deserializeJson( jb, buff );
                if ( err ) {
                    // Serial.print( F( "deserializeJson() failed: " ) );
                    // Serial.println( err.c_str() );
                }
                // activate the motors
                if ( jb[ "state" ] == 0 || jb[ "state" ] == 1 ) {
                    digitalWrite( SLP, HIGH );  // Wake up
                }
                // set the motors target
                if ( jb[ "device" ] = 0 ) {
                    for ( int i = 0; i < 5; i++ ) {
                        cameras[ i ].setYaw( jb[ "target" ][ i ] );
                    }
                }
                else if ( jb[ "device" ] = 1 ) {
                    for ( int i = 0; i < 5; i++ ) {
                        cameras[ i ].setPitch( jb[ "target" ][ i ] );
                    }
                }
                Serial.println( buff );
                buff = "";
            }
            else
                buff += inChar;
        }
    }
    else {
        for ( int i = 0; i < 2; i++ )
            Command[ i ] = 99;
    }
}