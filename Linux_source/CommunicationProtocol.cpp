//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

//************************************************************
// Name:      CommunicationProtocol.cpp
// Purpose:   Code for synchronising data packets
// Author:    Hennie Peters
// Created:   2021-08-20
// Copyright: Hennie Peters
// License:   the latest GNU General Public License
//************************************************************
#include "CommunicationProtocol.h"
#include "Serial.h"
#include "Fifo.h"

extern int comport_nr;

void send_byte( unsigned char c ) {
    if( comport_nr > 0 ) {
        RS232_SendByte( comport_nr, c );
    }
}

unsigned char read_byte( void ) {
    unsigned char c;
    unsigned char buf[64];
    while( true ) {
        if( ! fifo.IsEmpty() ) {
            c = fifo.Get();
            break;
        } else { // fifo is empty start polling
            int n;
            #ifdef __linux__
            usleep(1000);
            #else
            Sleep(1);
            #endif
            if( (n = RS232_PollComport( comport_nr, buf, 64 )) > 0 ) {
                for( int i=0; i<n; i++ ) {
                    fifo.Put( buf[i] );
                }
            }
        }
    }
    return c;
}

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
