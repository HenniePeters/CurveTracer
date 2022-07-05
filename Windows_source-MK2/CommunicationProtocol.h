//######################################################################################################
// this file is part of  https://github.com/HenniePeters/CurveTracer
//######################################################################################################
// Name:      CommunicationProtocol.h
// Purpose:   Code for synchronising data packets
// Author:    Hennie Peters
// Created:   2021-08-20
// Copyright: Hennie Peters
// License:   the latest GNU General Public License
//######################################################################################################
#ifndef CommunicationProtocolH
#define CommunicationProtocolH

#define BUFMAX 512
#define HEADER_BYTE1 0x55 // Bin:01010101  Dec:85 'U'
#define HEADER_BYTE2 0x33 // Bin:00110011  Dec:51 '3'

#define LSB( x ) ( x & 0x00FF )
#define MSB( x ) ( ( x & 0xFF00 ) >> 8 )
#define BYTESWAP16( x ) ( LSB( x ) << 8 ) + MSB( x )

#include <stdint.h>
//######################################################################################################
class communication_package {
  public:
    void pkg_clear( void );
    uint8_t pkg_add_byte( uint8_t );
    uint8_t pkg_add_uint16( uint16_t );
    uint16_t pkg_get_uint16( uint16_t idx );
    void pkg_send( void );
    uint8_t pkg_receive( void );
    uint8_t buf[BUFMAX]; // storage of data bytes
    uint16_t len;        // number of data bytes in communication package including the xor byte
    uint8_t chk;         // xor check of data including the length byte excluding the xor byte itself
};
//######################################################################################################
#endif
