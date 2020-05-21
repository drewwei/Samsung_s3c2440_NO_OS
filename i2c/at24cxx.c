#include "../include/int.h"
#include "../lib/string.h"
#include "../include/stdio.h"
#include "i2c.h"
#include "i2c_core.h"



#if 1

/*AT24C02写函数*/
void AT24CXX_i2c_write_OneByte(unsigned char add, unsigned char data)
{
	DEBUG("%s,%d,%s.\r\n",__FILE__,__LINE__,__func__);
	struct i2c_msg msg;  	/*构造一个msg*/
	unsigned char buf[2];
	/*
	 * 发从设备地址和读写状态不在msg数据范围内，它在do_do_master_rx和do_master_tx中已经做完了
	 * buf[0]为设备内存地址
	 * buf[1]为要写的数据
	 */
		buf[0] = add;
		buf[1] = data;
		
		/* 填充i2c_msg */
		msg.addr  = 0x50;
		msg.flags = TX; /* write */
		msg.len   = 2;
		msg.buf   = buf;
		msg.cnt_transferred = -1;
		i2c_transfer(&msg, 1);
}



/*AT24C02读函数，需要构造两个msg ,一个发(发要读的eeprom地址)，另一个收*/
void AT24CXX_i2c_read_OneByte(unsigned char add, unsigned char  *buffer)
{
   /*
	* 读数据前必须发出eeprom的内存地址，msg[0].flags = TX, msg[0].buf= 内存地址
	* 
	*/
	DEBUG("%s,%d,%s.\r\n",__FILE__,__LINE__,__func__);
	struct i2c_msg msgs[2];
	msgs[0].addr  = 0x50;
	msgs[0].flags = TX;
	msgs[0].len   = 1;
	msgs[0].buf   = &add;
	msgs[0].cnt_transferred = -1;
	msgs[1].addr  = 0x50;
	msgs[1].flags = RX;
	msgs[1].len   = 1;
	msgs[1].buf   = buffer;
	msgs[1].cnt_transferred = -1;
	i2c_transfer(msgs, 2);
}

#endif 