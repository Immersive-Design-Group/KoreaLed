#pragma once

#include "Motor.hpp"
#include <ArduinoJson.h>

static int Command[ 2 ];
String     buff = "";
// {"state":0,"target":[100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]}

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
                Command[ 0 ] = jb[ "state" ];
                if ( jb[ "state" ] == 0 ) {
                    digitalWrite( SLP, HIGH );  // Wake up
                }
                for ( int i = 0; i < 5; i++ ) {
                    cameras[ i ].setYaw( jb[ "target" ][ 2 * i ] );
                    cameras[ i ].setPitch( jb[ "target" ][ 2 * i + 1 ] );
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