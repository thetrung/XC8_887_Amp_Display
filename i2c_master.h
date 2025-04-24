#ifndef I2C_MASTER_H
#define	I2C_MASTER_H

#include "commons.h"

#define I2C_MODE_WRITE 0x00
#define I2C_MODE_READ 0x01

void i2c_init(const uint32_t c) {
    
    //Set SDA and SCL pins as input pins
//    PORTC = 0x00; // enable only C3-4
    TRISC3 = 1; // SCL
    TRISC4 = 1; // SDA 
    
    /*
    * SSPCON1:
    *  - WCOL[7]: 0 (No collision)
    *  - SSPOV[6]: 0 (No overflow)
    *  - SSPEN[5]: 1 (SSP Enabled)
    *  - CKP[4]: 0 (Unused)
    *  - SSPM[3-0]: 1000 (I2C Master mode)
    */
    SSPCON  = 0b00101000;    //pg84/234;

    /*
    * SSPCON2: 0 (Default settings)
    */
    SSPCON2 = 0b00000000;    //pg85/234;
    
    //SSPADD (Frequency)
    SSPADD = (u8)(_XTAL_FREQ / (4 * c)) - 1;
//  SSPADD = (_XTAL_FREQ/(4*feq_K*100))-1; //Setting Clock Speed pg99/234
    SSPSTAT = 0b00000000;    //pg83/234
}

void i2c_hold(void) {
    while (   (SSPCON2 & 0b00011111)    ||    (SSPSTAT & 0b00000100)   ) ; 
    //check the this on registers to make sure the IIC is not in progress
}

void i2c_start(void) {
    i2c_hold();
    SEN = 1;
}

void i2c_repeated_start(void) {
    i2c_hold();
    RSEN = 1;
}

void i2c_stop(void) {
    i2c_hold();
    PEN = 1;
}

void i2c_write(u8 data) {
    i2c_hold();
    SSPBUF = data;
}

void i2c_master_address(u8 address, u8 mode) {
    u8 addressWithMode = (u8)(address << 1);
    addressWithMode += mode;
    i2c_hold();
    SSPBUF = addressWithMode;
}


unsigned char i2c_read(u8 a) {
    u8 temp;
    i2c_hold();
    RCEN = 1;
    i2c_hold();
    temp = SSPBUF;
    i2c_hold();
    ACKDT = (a) ? 0 : 1;
    ACKEN = 1;
    return temp;
}

#endif