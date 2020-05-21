#include "../include/stdio.h"
#include "../include/s3c24xx.h"
#include "../include/int.h"
#include "../lib/string.h"
#include "../include/timer.h"
#include "i2c.h"



void (*i2c_transfer)(struct i2c_msg *msgs,unsigned int cnt);

struct i2c_controller *i2c_controller_arry[10];

/*1.注册主机控制器*/
int register_i2c_controller(struct i2c_controller *p_i2c_controller)
{
	DEBUG("%s,%d,%s.\r\n",__FILE__,__LINE__,__func__);
	int i;
	for(i=0; i<10; i++)
	{
		if(!i2c_controller_arry[i])
		{
			i2c_controller_arry[i] = p_i2c_controller;
			return 0;
		}			
	}
	return -1;
}
/*按名字找到自己想要的I2C控制器*/
struct i2c_controller * mach_i2c_controller(const char * name)
{
	DEBUG("%s,%d,%s.\r\n",__FILE__,__LINE__,__func__);
	int i,ret = -1;
	for(i=0; i<10; i++) {
		if(i2c_controller_arry[i] != NULL) {
			ret = strcmp(name,i2c_controller_arry[i]->name);
			if(ret == 0) {
				i2c_controller_arry[i]->init();
				i2c_transfer = i2c_controller_arry[i]->master_xfer;
				return i2c_controller_arry[i];				
			}
			
		}	
	}
	return NULL;
}
