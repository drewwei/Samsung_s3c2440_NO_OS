#include "spi_flash.h"
#include "timer.h"
#include "int.h"
#include "stdio.h"

void spi_flash(void)
{
	/*
	unsigned char c;
			uart_send("please enter 'r' or 'w' for read and write\r\n");
			c = getc();
			switch(c)
			{
			case 'w':
				//WriteData(0, gImage_picture, 305280);
				break;
			case 'y':
				//WriteData(305280+345600+259200, gImage_3, 307200);
				break;
			case 'r':
				
				SPIReadData(0, (unsigned char *)0x33c00000, 305280);
				mdelay(1000);
				SPIReadData(305280, (unsigned char *)0x33c00000, 345600);
				mdelay(1000);
				SPIReadData(305280+345600, (unsigned char *)0x33c00000, 259200);
				mdelay(1000);
				SPIReadData(305280+345600+259200, (unsigned char *)0x33c00000, 307200);
				break;

			}
	*/
	SPIReadData(0, (unsigned char *)0x33c00000, 305280);
	mdelay(1000);
	SPIReadData(305280, (unsigned char *)0x33c00000, 345600);
	mdelay(1000);
	SPIReadData(305280+345600, (unsigned char *)0x33c00000, 259200);
	mdelay(1000);
	SPIReadData(305280+345600+259200, (unsigned char *)0x33c00000, 307200);	
	mdelay(1000);
}






















