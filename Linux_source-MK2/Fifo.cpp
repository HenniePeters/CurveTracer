#include "Fifo.h"

//------------------------------------------------------------------------
FifoClass::FifoClass() {
    head = 0;
    tail = 0;
    char_count = 0;
}
//------------------------------------------------------------------------
FifoClass::~FifoClass() {
    //
}
//------------------------------------------------------------------------
bool FifoClass::IsFull( void ) {
    uint16_t new_head = ( head + 1 ) & FIFO_BUFFER_MASK;
    return( new_head != tail );
}
//------------------------------------------------------------------------
bool FifoClass::IsEmpty( void ) {
    return( head == tail );
}
//------------------------------------------------------------------------
void FifoClass::Clear( void ) {
    head = tail;
    char_count = 0;
}
//------------------------------------------------------------------------
uint16_t FifoClass::Size( void ) {
    return char_count;
}
//------------------------------------------------------------------------
bool FifoClass::Put( unsigned char c ) {
    uint16_t new_head = ( head + 1 ) & FIFO_BUFFER_MASK;
    if( new_head == tail ) {
        return false; // overflow
    } else {
        buf[head] = c;
        head = new_head;
        char_count++;
        return true;
    }
}
//------------------------------------------------------------------------
unsigned char FifoClass::Get( void ) {
    unsigned char c = 0;
    if( ! IsEmpty() ) {
        c = buf[tail];
        tail = ( tail + 1 ) & FIFO_BUFFER_MASK;
        if( head == tail ) {
            char_count = 0;
        } else {
            char_count--;
        }
    }
    return c;
}
//------------------------------------------------------------------------
