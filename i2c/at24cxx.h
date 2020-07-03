

//extern void AT24CXX_i2c_write(unsigned char add, unsigned char * data, unsigned int len);
//extern void AT24CXX_i2c_read(unsigned char add, unsigned char * data, unsigned int len);
extern void AT24CXX_i2c_read_OneByte(unsigned char add, unsigned char  *buffer);
/*AT24C02写函数*/
extern void AT24CXX_i2c_write_OneByte(unsigned char add, unsigned char data);

extern void AT24CXX_i2c_read(unsigned char addr, unsigned char * data, unsigned int len);

extern void AT24CXX_i2c_write(unsigned char addr, unsigned char* data, unsigned int len);

extern void i2c_app(void);