/*
 * 20190312.006
 * OLED 128x32
 *
 * File: I2CMaster.h
 * Processor: PIC16F1825
 * Author: wizlab.it
 */

#ifndef I2CMASTER_H
#define	I2CMASTER_H

#include "commons.h"

#define I2C_MODE_WRITE 0x00
#define I2C_MODE_READ 0x01

void I2C_Master_Init(const uint32_t c);
void I2C_Master_Wait(void);
void I2C_Master_Start(void);
void I2C_Master_RepeatedStart(void);
void I2C_Master_Stop(void);
void I2C_Master_Address(u8 address, u8 mode);
void I2C_Master_Write(u8 d);
u8 I2C_Master_Read(u8 a);

#endif