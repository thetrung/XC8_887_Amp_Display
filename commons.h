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

#define _XTAL_FREQ 16000000     //CPU Frequency
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

// FUNCTIONS 
#define pinMode(PIN, MODE) (PIN = MODE)
#define digitalWrite(PORT, STATE) (PORT = STATE)
#define min(x1, x2) (x1 < x2 ? x1 : x2)
#define max(x1, x2) (x1 > x2 ? x1 : x2)
// BIT FLAGS
#define setFlag(flag, n) ((flag) |= (1 << n))
#define clearFlag(flag, n) ((flag) &= ~(1 << n))
#define getFlag(flag, n) (((flag) >> (n)) & 1)

#define delay __delay_ms


#endif