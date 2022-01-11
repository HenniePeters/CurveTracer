//---------------------------------------------------------------------------
// this file is part of  https://github.com/HenniePeters/CurveTracer
//---------------------------------------------------------------------------

#ifndef FifoH
#define FifoH

#include <stdint.h>
//#include <stdbool.h>

//  Fifo buffer size: 2,4,8,16,32,64,128 ..... 65536 bytes.
#define FIFO_BUFFER_SIZE 65536

//  Fifo buffer mask.
#define FIFO_BUFFER_MASK ( FIFO_BUFFER_SIZE - 1 )

#if ( FIFO_BUFFER_SIZE > 65536 )
#error Fifo buffer size does NOT fit in a uint16_t, choose: 2,4,8,16,32,64,128 ..... 65536 bytes.
#endif

#if ( FIFO_BUFFER_SIZE & FIFO_BUFFER_MASK )
#error Fifo buffer size is not a power of 2
#endif

class FifoClass {
public:
    FifoClass();
    ~FifoClass();
    bool IsFull( void );
    bool IsEmpty( void );
    uint16_t Size( void );
    void Clear( void );
    bool Put( unsigned char c );
    unsigned char Get( void );
private:
    volatile uint16_t buf[FIFO_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t char_count;
};

extern class FifoClass fifo;

#endif //Fifo_H


