#ifndef I2CMASTER_H
#define	I2CMASTER_H

#include "commons.h"

#define I2C_MODE_WRITE 0x00
#define I2C_MODE_READ 0x01

void i2c_init(const uint32_t c);
void i2c_hold(void);
void i2c_start(void);
void i2c_repeated_start(void);
void i2c_stop(void);
void i2c_master_address(u8 address, u8 mode);
void i2c_write(u8 d);
u8 i2c_read(u8 a);

#endif