/*
 * 20190312.006
 * OLED 128x32 -> 128x64
 *
 * File: init.c
 * Processor: PIC16F1825 -> PIC18F45k50
 * Author: wizlab.it && modded by thetrung.
 * 
 * Now run on MPLAB X IDE 6.20+ && PICKIT5
 */

#include "init.h"


/*==============================================================================
 * System initialization
 *============================================================================*/
void init(void) {
    /*
    * Init Oscillator: 16MHz
    *  - SPLLEN[7]: 0 (4x PLL enabled)
    *  - IRCF[6-4]: 111 (8MHz/32MHz)
    *  - SCS[1-0]: 00 (Clock by FOSC [FOSC = INTOSC])
    */
//    OSCCON = 0x00; // 0b0000 0000
//    OSCCONbits.IRCF = 0b111; // IRCF = 0b111 => 8 Mhz
//    OSCCONbits.SCS = 0b11; // using external high-speed crystal @ 16Mhz
    OSCCONbits.SCS = 0b00;  // Select OSC source = FOSC.
    /*
    * Initialize I2C bus
    */
    i2c_init(400000);

    /*
    * Interrupts
    * TMR0IE: 0 (Timer0 interrupt disabled)
    * SSP1 (I2C): 0 (SSP1 interrupt initially disabled)
    * PEIE: 1 (Peripheral interrupts disabled)
    * GIE: 1 (Global interrupts disabled)
    */
//    TMR0IE = 0;
//    SSPIE = 0;
//    PEIE = 0;
//    GIE = 0;
}


/*==============================================================================
 * Interrupt Service Routine
 *============================================================================*/
void __interrupt() isr(void) {
}