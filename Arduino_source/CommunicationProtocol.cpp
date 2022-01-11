#include <stdio.h>
#include "CommunicationProtocol.h"

unsigned char read_byte( void ) {
    unsigned char c = Serial.read();
    return c;
}
//######################################################################################################
void send_byte( unsigned char c ) {
    Serial.write( c );
}
//######################################################################################################
/*
void printbyte( unsigned char c ) {
    char buf[256];
    sprintf( buf, "%02X", c );
    Serial.print( buf );
}
void pkg_send( com_pkg *pkg ) { // DEBUG VERSION
    Serial.println("");
    printbyte( HEADER_BYTE1 );
    printbyte( HEADER_BYTE2 );
    printbyte( pkg->len );
    if( pkg->len == HEADER_BYTE1 ) {
        printbyte( HEADER_BYTE1 );
    }
    for( unsigned int i=0; i<pkg->len; i++ ) {
        printbyte( pkg->buf[i] );
        if( pkg->buf[i] == HEADER_BYTE1 ) {
            printbyte( HEADER_BYTE1 );
        }
    }
    printbyte( pkg->chk );
    if( pkg->chk == HEADER_BYTE1 ) {
        printbyte( HEADER_BYTE1 );
    }
}
*/
//######################################################################################################
//######################################################################################################
//######################################################################################################
//######################################################################################################
void communication_package::pkg_clear( void ) {
    len = 0;
    chk = 0;
}
//######################################################################################################
uint8_t communication_package::pkg_add_byte( uint8_t byt ) {
    uint16_t l = len;
    if( ++l > BUFMAX ) {
        return 1; // OVERFLOW
    } else {
        buf[len] = byt;
        chk ^= (len & 0xFF); // revert xor of old length
        chk ^= (len >> 8);
        len++;      // raise lenght by one
        chk ^= (len & 0xFF); // xor with new lenght
        chk ^= (len >> 8);
        chk ^= byt;
        return 0;
    }
}
//######################################################################################################
void communication_package::pkg_send( void ) {
    send_byte( HEADER_BYTE1 );
    send_byte( HEADER_BYTE2 );
    uint8_t l = ( len & 0xFF );
    send_byte( l );
    if( l == HEADER_BYTE1 ) {
        send_byte( HEADER_BYTE1 );
    }
    l = ( len >> 8 );
    send_byte( l );
    if( l == HEADER_BYTE1 ) {
        send_byte( HEADER_BYTE1 );
    }
    for( uint16_t i = 0; i < len; i++ ) {
        send_byte( buf[i] );
        if( buf[i] == HEADER_BYTE1 ) {
            send_byte( HEADER_BYTE1 );
        }
    }
    send_byte( chk );
    if( chk == HEADER_BYTE1 ) {
        send_byte( HEADER_BYTE1 );
    }
}
//######################################################################################################
uint8_t communication_package::pkg_receive( void ) {
    uint8_t byt;
    byt = read_byte();
    if( byt != HEADER_BYTE1 ) {
        return -1;
    } else {
        byt = read_byte();
        if( byt != HEADER_BYTE2 ) {
            return 1;
        } else {
            byt = read_byte();
            if( byt == HEADER_BYTE1 ) {
                byt = read_byte();
                if( byt != HEADER_BYTE1 ) {
                    return -1;
                }
            }
            len = (byt & 0xFF);
            chk = byt;
            byt = read_byte();
            if( byt == HEADER_BYTE1 ) {
                byt = read_byte();
                if( byt != HEADER_BYTE1 ) {
                    return 1;
                }
            }
            len += ( byt << 8 );
            chk ^= byt;
            for( uint16_t i = 0; i < len; i++ ) {
                byt = read_byte();
                if( byt == HEADER_BYTE1 ) {
                    byt = read_byte();
                    if( byt < 0 ) {
                        return 1;
                    }
                    if( byt != HEADER_BYTE1 ) {
                        return 1;
                    }
                }
                buf[i] = byt;
                chk ^= byt;
            }
            byt = read_byte();
            if( byt == HEADER_BYTE1 ) {
                byt = read_byte();
                if( byt != HEADER_BYTE1 ) {
                    return 1;
                }
            }
            if( byt != chk ) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}
//######################################################################################################
