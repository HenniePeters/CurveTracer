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
union TwoBytes {
    uint16_t u16;
    uint8_t u8[2];
};
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
        len++;      // raise lenght by one
        return 0;
    }
}
//######################################################################################################
uint8_t communication_package::pkg_add_uint16( uint16_t param ) {
    if( this->len + 2 > BUFMAX ) {
        return 1; // OVERFLOW
    } else {
        this->buf[this->len] = MSB(param); // store new data
        this->buf[this->len + 1] = LSB(param);
        this->len += 2;       // raise lenght
        return 0;
    }
}
//######################################################################################################
uint16_t communication_package::pkg_get_uint16( uint16_t idx ) {
    return (this->buf[idx] << 8) + this->buf[idx + 1];
}
//######################################################################################################
void communication_package::pkg_send( void ) {
    uint8_t chk;
    send_byte( HEADER_BYTE1 );
    send_byte( HEADER_BYTE2 );
    uint8_t tmp = MSB(this->len);
    chk = tmp;
    send_byte( tmp );
    if( tmp == HEADER_BYTE1 ) {
        send_byte( HEADER_BYTE1 );
    }
    tmp = LSB(this->len);
    chk ^= tmp;
    send_byte( tmp );
    if( tmp == HEADER_BYTE1 ) {
        send_byte( HEADER_BYTE1 );
    }
    for( uint16_t i = 0; i < this->len; i++ ) {
        chk ^= this->buf[i];
        send_byte( this->buf[i] );
        if( this->buf[i] == HEADER_BYTE1 ) {
            send_byte( HEADER_BYTE1 );
        }
    }
    this->chk = chk;
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
        return 1;
    } else {
        byt = read_byte();
        if( byt != HEADER_BYTE2 ) {
            return 1;
        } else { // header complete, next receive the length: 2 bytes
            byt = read_byte();
            if( byt == HEADER_BYTE1 ) {
                byt = read_byte();
                if( byt != HEADER_BYTE1 ) {
                    return 1;
                }
            }
            this->len = (byt << 8);
            this->chk = byt;
            byt = read_byte();
            if( byt == HEADER_BYTE1 ) {
                byt = read_byte();
                if( byt != HEADER_BYTE1 ) {
                    return 1;
                }
            }
            this->len += byt;
            this->chk ^= byt;
            for( uint16_t i = 0; i < this->len; i++ ) { // receive the data buffer
                byt = read_byte();
                if( byt == HEADER_BYTE1 ) {
                    byt = read_byte();
                    if( byt != HEADER_BYTE1 ) {
                        return 1;
                    }
                }
                this->buf[i] = byt;
                this->chk ^= byt;
            }
            byt = read_byte(); // receive the checksum
            if( byt == HEADER_BYTE1 ) {
                byt = read_byte();
                if( byt != HEADER_BYTE1 ) {
                    return 1;
                }
            }
            if( byt != this->chk ) {
                return 1; /// 1; // checksum error
            } else {
                return 0; // reception complete
            }
        }
    }
}
//######################################################################################################
