//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

//************************************************************
// Name:      CommunicationProtocol.h
// Purpose:   Code for synchronising data packets
// Author:    Hennie Peters (hennie[dot]peters[at]gmail[dot]com)
// Created:   2021-08-20
// Copyright: Hennie Peters
// License:   the latest GNU General Public License
//************************************************************

#define BUFMAX        300
#define HEADER_BYTE1 0xFF
#define HEADER_BYTE2 0x00

#include <stdint.h>

//######################################################################################################
class communication_package {
public:
    void pkg_clear( void );
    uint8_t pkg_add_byte( uint8_t );
    void pkg_send( void );
    uint8_t pkg_receive( void );
    uint8_t buf[BUFMAX]; // storage of data bytes
    uint16_t len;           // number of data bytes in communication package including the xor byte
    uint8_t chk;            // xor check of data including the length byte excluding the xor byte itself
};
//######################################################################################################

