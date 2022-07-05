#include <Arduino.h>
#include <stdint.h>

#define BUFMAX        512 
#define HEADER_BYTE1 0x55
#define HEADER_BYTE2 0x33

#define LSB(x)   ( x & 0x00FF )
#define MSB(x) ( ( x & 0xFF00 ) >> 8 )

//######################################################################################################
class communication_package {
public:
    void pkg_clear( void );
    uint8_t pkg_add_byte( uint8_t );
    uint8_t pkg_add_uint16( uint16_t param );
    uint16_t pkg_get_uint16( uint16_t idx );
    void pkg_send( void );
    uint8_t pkg_receive( void );
    
    uint8_t buf[BUFMAX];    // storage of data bytes
    uint16_t len;           // number of data bytes in communication package including the xor byte
    uint8_t chk;            // xor check of data (contents of buf[]) including the length byte excluding the xor byte itself
};
//######################################################################################################
