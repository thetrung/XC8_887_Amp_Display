/* NOTE :: thetrung @ 19th April, 2025 ::
 *  
 * Convert 45K50 firmware -> 16F887
 */

#ifndef COMMONS_H
#define COMMONS_H

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define _XTAL_FREQ 8000000     //CPU Frequency
// uintX_t
#define u8 uint8_t
#define u16 uint16_t
// intX_t
#define i8 int8_t
#define i16 int16_t

// MODE
#define LOW 0
#define HIGH 1

// I/O
#define INPUT 1
#define OUTPUT 0

// PORT-A
#define RA0 TRISA0
#define RA1 TRISA1
#define RA2 TRISA2
#define RA3 TRISA3
#define RA4 TRISA4
#define RA5 TRISA5
#define RA6 TRISA6
#define RA7 TRISA7

// FUNCTIONS 
#define pinMode(PIN, MODE) (PIN = MODE)
#define digitalWrite(PORT, STATE) (PORT = STATE)
#define min(x1, x2) (x1 < x2 ? x1 : x2)
#define max(x1, x2) (x1 > x2 ? x1 : x2)

#define delay __delay_ms

//External functions
extern void init(void);

#endif