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
				
				SpiFlashReadData(0, (unsigned char *)0x33c00000, 305280);
				mdelay(1000);
				SpiFlashReadData(305280, (unsigned char *)0x33c00000, 345600);
				mdelay(1000);
				SpiFlashReadData(305280+345600, (unsigned char *)0x33c00000, 259200);
				mdelay(1000);
				SpiFlashReadData(305280+345600+259200, (unsigned char *)0x33c00000, 307200);
				break;

			}
	*/
	SpiFlashReadData(0, (unsigned char *)0x33c00000, 305280);
	mdelay(1000);
	SpiFlashReadData(305280, (unsigned char *)0x33c00000, 345600);
	mdelay(1000);
	SpiFlashReadData(305280+345600, (unsigned char *)0x33c00000, 259200);
	mdelay(1000);
	SpiFlashReadData(305280+345600+259200, (unsigned char *)0x33c00000, 307200);	
	mdelay(1000);
}


void spi_flash_app()
{
	const unsigned char buff1[] = "Everything will be ok!";
	unsigned char buff2[30];
	int i;
	char c;
	//unsigned char buff3 = 0x3;
	while(1)
	{
		c = getc();
		switch (c)
		{
		case 'w':
		case 'W':
			SpiFlashWriteData(0, buff1, sizeof(buff1));
			printf("SpiFlashWriteData\r\n");
			break;
		case 'r':
		case 'R':
			SpiFlashReadData(0, buff2, 30);
			for(i = 0; i < 30; i++)
			{
				printf("%c ", buff2[i]);
			}
			printf("\r\n");
			break;
		default:
			break;
		}		
	}
}



















