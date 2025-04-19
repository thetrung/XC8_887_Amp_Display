#include "I2CMaster.h"

void I2C_Master_Init(const uint32_t c) {
    /*
    * SSPCON1:
    *  - WCOL[7]: 0 (No collision)
    *  - SSPOV[6]: 0 (No overflow)
    *  - SSPEN[5]: 1 (SSP Enabled)
    *  - CKP[4]: 0 (Unused)
    *  - SSPM[3-0]: 1000 (I2C Master mode)
    */
    SSPCON = 0b00101000;

    /*
    * SSPCON2: 0 (Default settings)
    * SSPCON3: 0 (Default settings)
    * SSPSTAT: 0 (Default settings)
    */
    SSPCON2 = 0;
    SSPSTAT = 0;

    //SSPADD (Frequency)
    SSPADD = (_XTAL_FREQ / (4 * c)) - 1;
}

void I2C_Master_Wait(void) {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start(void) {
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Master_RepeatedStart(void) {
    I2C_Master_Wait();
    RSEN = 1;
}

void I2C_Master_Stop(void) {
    I2C_Master_Wait();
    PEN = 1;
}

void I2C_Master_Address(u8 address, u8 mode) {
    u8 addressWithMode;
    addressWithMode = address << 1;
    addressWithMode += mode;
    I2C_Master_Wait();
    SSPBUF = addressWithMode;
}

void I2C_Master_Write(u8 d) {
    I2C_Master_Wait();
    SSPBUF = d;
}

unsigned char I2C_Master_Read(u8 a) {
    uint8_t temp;
    I2C_Master_Wait();
    RCEN = 1;
    I2C_Master_Wait();
    temp = SSPBUF;
    I2C_Master_Wait();
    ACKDT = (a) ? 0 : 1;
    ACKEN = 1;
    return temp;
}