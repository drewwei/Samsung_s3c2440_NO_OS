#include "at24cxx.h"
#include "../include/stdio.h"
void i2c_app(void)
{
    unsigned char data0,data1 = 7,c;
	while(1)
	{
		printf("please enter 'r' or 'w' for read and write\r\n");
		c = getc();
		switch(c)
		{
			case 'w':
				printf("please enter a char.\r\n");
				data0 = getc();
#ifdef IIC_INT
				AT24CXX_i2c_write_OneByte(1,data0);
#else
				AT24CXX_i2c_write(1, &data0, 1);
#endif
				break;
			case 'r':
#ifdef IIC_INT
				AT24CXX_i2c_read_OneByte(1,&data1);
#else
				AT24CXX_i2c_read(1,&data1, 1);
#endif
				printf("data1:%d\r\n",data1);
				break;
			default:
				break;
		}	
	
	}
}