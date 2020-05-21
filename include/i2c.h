//====================================================================
// File Name : 2410IIC.h
// Function  : S3C2410 IIC Test Program Head file
// Program   : Shin, On Pil (SOP)
// Date      : March 20, 2002
// Version   : 0.0
// History
//   0.0 : Programming start (March 11, 2002) -> SOP
//====================================================================

#ifndef __2440IIC_H__
#define __2440IIC_H__

void i2c_handler(void);
void AT24CXX_i2c_read(unsigned char addr, unsigned char * data, unsigned int len);
void AT24CXX_i2c_write(unsigned char addr, unsigned char* data, unsigned int len);
extern void init_s3c2440_i2c_controller(void);
extern int register_i2c_controller(struct i2c_controller *p_i2c_controller);




struct i2c_msg {
	unsigned int addr;   /*7bit device address*/
	enum {
		TX,
		RX,
	}flags;
	int len;
	int cnt_transferred;
	unsigned char *buf;
};
struct i2c_controller {
	void (*init)(void);
	void (*master_xfer)(struct i2c_msg * , unsigned int );
	const char *name;
};

#endif    //__2440IIC_H__
