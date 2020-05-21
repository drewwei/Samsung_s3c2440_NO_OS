
#ifndef		__S3C2440_I2C__
#define		__S3C2440_I2C_

#include "i2c.h"

extern void i2c_irq(void);
extern void AT24CXX_i2c_read(unsigned char addr, unsigned char * data, unsigned int len);
extern void AT24CXX_i2c_write(unsigned char addr, unsigned char* data, unsigned int len);
extern void init_s3c2440_i2c_controller(struct i2c_controller *s3c2440_i2c_controller);

#endif