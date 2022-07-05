#include <mem.h>
#include "CommunicationProtocol.h"
extern uint8_t send_buf( uint8_t *byt, uint16_t bufsize );
extern uint8_t send_byte( uint8_t byt );
extern uint8_t read_byte( void );

//######################################################################################################
//
// REMARKS, Hennie Peters, 2022-05-29
//
// Modification to speed up communication, the checksum is now calculated during transmission.
// Prior to this version it was calculated every time a byte was added to the structure, keeping the checksum correct at
// all times. Because the checksum is sent as the last byte of the package, it can easily be calculated during
// transmission. The checksum of the package is now correct at the end of the functions "pkg_send" and "pkg_receive".
//
//######################################################################################################
//
// REMARKS, Hennie Peters, 2022-05-20
////
// A single data byte 0x55 will result in these 7 data bytes sent over the communication line/medium:
// Package contents:
// 55 33 00 01 55 55 54
// 55 33                    <-  the header containing bytes: 0x55 = Bin:01010101 Dec:85 'U' and 0x33 = Bin:00110011
// Dec:51 '3'
//       00 01              <-  the number of data bytes, MSB first, LSB last (macros MSB and LSB are defined in
//       CommunicationProtocol.h)
//             55 55        <-  a single data byte (doubled because it contains a value equal to the first header byte)
//                   54     <-  checksum byte (length and databytes XOR-ed)
//                              Mind that bytes equal to the first header byte are sent twice but not XOR-ed twice!
//
//######################################################################################################
//
//######################################################################################################
//
// REMARKS, Hennie Peters, 2022-04-14
//
// Fifo size (FIFO_SIZE in fifo.h) should have sufficient space.
// Protocol buffer (BUFMAX in CommunicationProtocol.h) should have sufficient space.
// Mind that (fifo)buffer size needs space for communication_package.size + 5 ALSO data bytes equal to headerbyte1 need
// to be doubled!
//
//######################################################################################################

//######################################################################################################
void communication_package::pkg_clear( void ) {
    len = 0;
    chk = 0;
    memset( buf, 0xFF, BUFMAX );
}
//######################################################################################################
uint8_t communication_package::pkg_add_byte( uint8_t byt ) {
    uint16_t l = len;
    if ( ++l > BUFMAX ) {
        return 1; // OVERFLOW
    } else {
        buf[len] = byt;
        len++; // raise lenght by one
        return 0;
    }
}
//######################################################################################################
uint16_t communication_package::pkg_get_uint16( uint16_t idx ) {
    return ( this->buf[idx] << 8 ) + this->buf[idx + 1];
}
//######################################################################################################
uint8_t communication_package::pkg_add_uint16( uint16_t param ) {
    if ( this->len + 2 > BUFMAX ) {
        return 1; // OVERFLOW
    } else {
        this->buf[this->len] = MSB( param ); // store new data
        this->buf[this->len + 1] = LSB( param );
        this->len += 2; // raise lenght
        return 0;
    }
}
//######################################################################################################
void communication_package::pkg_send( void ) {
    uint8_t chk;
    send_byte( HEADER_BYTE1 );
    send_byte( HEADER_BYTE2 );
    uint8_t tmp = MSB( this->len );
    chk = tmp;
    send_byte( tmp );
    if ( tmp == HEADER_BYTE1 ) {
        send_byte( HEADER_BYTE1 );
    }
    tmp = LSB( this->len );
    chk ^= tmp;
    send_byte( tmp );
    if ( tmp == HEADER_BYTE1 ) {
        send_byte( HEADER_BYTE1 );
    }
    for ( uint16_t i = 0; i < this->len; i++ ) {
        chk ^= this->buf[i];
        send_byte( this->buf[i] );
        if ( this->buf[i] == HEADER_BYTE1 ) {
            send_byte( HEADER_BYTE1 );
        }
    }
    this->chk = chk;
    send_byte( chk );
    if ( chk == HEADER_BYTE1 ) {
        send_byte( HEADER_BYTE1 );
    }
}
//######################################################################################################
uint8_t communication_package::pkg_receive( void ) {
    uint8_t byt;
    byt = read_byte();
    if ( byt != HEADER_BYTE1 ) {
        return 1;
    } else {
        byt = read_byte();
        if ( byt != HEADER_BYTE2 ) {
            return 1;
        } else { // header complete, next receive the length: 2 bytes
            byt = read_byte();
            if ( byt == HEADER_BYTE1 ) {
                byt = read_byte();
                if ( byt != HEADER_BYTE1 ) {
                    return 1;
                }
            }
            this->len = ( byt << 8 );
            this->chk = byt;
            byt = read_byte();
            if ( byt == HEADER_BYTE1 ) {
                byt = read_byte();
                if ( byt != HEADER_BYTE1 ) {
                    return 1;
                }
            }
            this->len += byt;
            this->chk ^= byt;
            for ( uint16_t i = 0; i < this->len; i++ ) { // receive the data buffer
                byt = read_byte();
                if ( byt == HEADER_BYTE1 ) {
                    byt = read_byte();
                    if ( byt != HEADER_BYTE1 ) {
                        return 1;
                    }
                }
                this->buf[i] = byt;
                this->chk ^= byt;
            }
            byt = read_byte(); // receive the checksum
            if ( byt == HEADER_BYTE1 ) {
                byt = read_byte();
                if ( byt != HEADER_BYTE1 ) {
                    return 1;
                }
            }
            if ( byt != this->chk ) {
                return 0; /// 1; // checksum error
            } else {
                return 0; // reception complete
            }
        }
    }
}
//######################################################################################################
