// https://www.arduino.cc/reference/en/libraries/spi-dac7611/
// http://www.analog.com/media/en/technical-documentation/data-sheets/AD5570.pdf
// MCP 4821 4822 4921 4922 DA controller 12 bit
#include "CommunicationProtocol.h"

#define DEBUGGING   0               // 0 for normal operation, set to 1 when debugging the arduino with no electronics connected (I used it to debug the PC software).
#define COORDS      50              // number of measurements per period
#define DELAY       20000/COORDS    // 420 microseconds (=20000us/COORDS) delay between measurements (assuming 50Hz net frequency) for 60 Hz use 347 microseconds (=16667us/COORDS)
#define PIN_30V     A0              // PC0 ADC0 measure the baselineline for the sinusoidal wave.
#define PIN_TRAFO   A1              // Not used yet, to do: automatic scaling
#define PIN_X       A2              // Resistor devided X-connection for oscilloscope  
#define PIN_Y       A3              // Resistor devided Y-connection for oscilloscope
#define PIN_INT0    2               // Used to detect the zero crossing of the sine
#define FACTOR_MUL  370             // multiplier: 220k + 150k resistors (calculation in integers to avoid excessive memory usage of the floating point library)
#define FACTOR_DIV  150             // divider: 150k resistor
#define ADC_MAXVAL  1023            // AVR(10bits):1023, STM(12bits) 4095
#define X_MAXVAL    3000            // Maximum scale value horizontal in mV (minimum value is 0-X_MAXVAL)
#define Y_MAXVAL    3000            // Maximum scale value vertical in mV   (minimum value is 0-Y_MAXVAL)
#define VCC         3460            // measured VCC 3460 mV

struct f {
    uint16_t endianness;
    uint16_t Vx, Vy;            // max scale value in mV (positive or negative)
    uint16_t coords;            // number of coordinates stored in x,y
    uint16_t freq;              // frequency in Hz
    int16_t xy[(COORDS<<1)];    // sample data
};

volatile bool startpulse;          // set by int0 at the rising zero crossing
class communication_package *cp_out = new class communication_package;
struct f fields;
int32_t baseline;

/////////////////////////////////////////////////////////////////////////////////////////////
void int0() {
    startpulse = true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
    Serial.begin( 115200 );
    attachInterrupt( digitalPinToInterrupt( PIN_INT0 ), int0, RISING );
    fields.Vx = X_MAXVAL;
    fields.Vy = Y_MAXVAL;
    fields.freq = 50; // fixed
    int n = 1;
    if( *(char*) &n == 1 ) { // little endian if true
        fields.endianness = ( 'L' << 8 ) + 'L';
    } else {
        fields.endianness = ( 'B' << 8 ) + 'B';
    }
    fields.coords = COORDS;
}
/////////////////////////////////////////////////////////////////////////////////////////////
int32_t convert( uint16_t value ) {
    return ( ( int32_t )value - baseline ) * FACTOR_MUL * VCC / ADC_MAXVAL / FACTOR_DIV;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
    static uint32_t us, ms_counter = 0;
    // uint16_t arr_t[COORDS];
    baseline =  analogRead( PIN_30V );
    if( millis() >= ms_counter || millis() < 5 ) { // measure a full periode once per 100 ms
        ms_counter = millis() + 100;
        if( micros() < 0xFFFFFFFF - 21000 ) { // avoid overflow in uint32_t during measurements
#if DEBUGGING == 1 //if( DEBUGGING ) { // this shows a circle
            baseline = ( ADC_MAXVAL / 2 );
            int sinusx[] = { 0, 1044, 2070, 3061, 4000, 4870, 5656, 6346, 6928, 7391, 7727, 7931, 7999, 7931, 7727, 7391, 6928, 6346, 5656, 4870, 3999, 3061, 2070, 1044, -1, -1045, -2071, -3062, -4001, -4871, -5657, -6347, -6929, -7392, -7728, -7932, -8000, -7932, -7728, -7391, -6929, -6347, -5657, -4871, -4000, -3062, -2071, -1045 };
            int sinusy[] = { 7999, 7931, 7727, 7391, 6928, 6346, 5656, 4870, 3999, 3061, 2070, 1044, -1, -1045, -2071, -3062, -4001, -4871, -5657, -6347, -6929, -7392, -7728, -7932, -8000, -7932, -7728, -7391, -6929, -6347, -5657, -4871, -4000, -3062, -2071, -1045, 0, 1044, 2070, 3061, 4000, 4870, 5656, 6346, 6928, 7391, 7727, 7931 };
            for( int i = 0; i < COORDS; i++ ) {
                fields.y[i] = sinusy[i];  
                fields.x[i] = sinusx[i]; 
            }
#else // } else {
            startpulse = false;
            //while( startpulse == false ); // start at the zero crossing
            us = micros();
            for( int i = 0; i < (COORDS<<1); i+=2 ) {
                // no calculations within this for-loop
                // arr_t[i]  = analogRead( PIN_TRAFO );
                fields.xy[i]  = analogRead( PIN_X );
                fields.xy[i+1]  = analogRead( PIN_Y );
                us += DELAY; // wait 420 us for 50Hz, 347 us for 60Hz, change DELAY to match your net frequency
                while( micros() < us );
                // digitalWrite(10, HIGH); // To test with an oscilloscope whether the timing is within DELAY us
                // digitalWrite(10, LOW);  // I used pin 10 of an Arduino Pro Mini during development
            }
            for( int i = 0; i < (COORDS<<1); i+=2 ) { // convert ADC readings to millivolts
                fields.xy[i] = convert( fields.xy[i] );  
                fields.xy[i+1] = convert( fields.xy[i+1] );  
            }
#endif // }         
           
            unsigned char buffer[sizeof( struct f )];
            memcpy( &buffer, &fields, sizeof( struct f ) );
            cp_out->pkg_clear();
            for( int i = 0; i < sizeof( buffer ); i++ ) {
                if( cp_out->pkg_add_byte( buffer[i] ) ) {
                    break;
                }
            }
           cp_out->pkg_send();
           /*
           for( int i = 0; i < COORDS; i++ ) {
                Serial.print( fields.x[i] );
                Serial.print( " " );
           }
           Serial.println( " " );          
           for( int i = 0; i < COORDS; i++ ) {
                Serial.print( fields.y[i] );
                Serial.print( " " );
           }
           Serial.println( " " );
           */
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
