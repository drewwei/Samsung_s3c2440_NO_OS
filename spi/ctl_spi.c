#include "../include/s3c24xx.h"
//#include "../include/timer.h"
#include "spi_flash.h"
void GPIO_SPI_Init()
{
/*
 *GPF1作为OLED片选引脚，设置为输出；
 */
 
  GPFCON &= ~(3<<(1*2));
  GPFCON |= (1<<(1*2));
  GPFDAT |= (1<<1);
	
/*
 *GPG2作为FLASH片选引脚，设置为输出；
 *GPG4作为OLED的数据(Data)/命令(Command)选择引脚，设置为输出；
 *GPG5作为SPI的MISO，设置为SPIMISO；
 *GPG6作为SPI的MOSI，设置为SPIMOSI；
 *GPG7作为SPI的时钟CLK，设置为SPICLOCK；		
 */
	GPGCON &= ~((3<<(2*2)) | (3<<(4*2)) | (3<<(5*2)) | (3<<(6*2)) | (3<<(7*2)));
    GPGCON |= ((1<<(2*2)) | (1<<(4*2)) | (3<<(5*2)) | (3<<(6*2)) | (3<<(7*2)));
    GPGDAT |= (1<<2);
	
	SPCON1 &= ~((3<<5)|(1<<0));
	SPCON1 |= (1<<4) | (1<<3);
}

static void SPIStatus()
{
	int count = 0xffff;
	while(!(SPSTA1 & 1))
	{
		if(!count)	break;
		count--;
	}		
}
static void SetClockMode(int mode)
{
	switch(mode)
	{
		case 0:
			SPCON1 &= ~(3<<1);
			break;
		case 1:			
			SPCON1 &= ~(1<<2);
			SPCON1 |= (1<<1);
			break;
		case 2:
			SPCON1 &= ~(1<<1);
			SPCON1 |= (1<<2);	
			break;
		case 3:		
			SPCON1 |= (3<<1);	
			break;
		default:
			break;
	}
}

static void SetPrescalerVal()
{
	SPPRE1 = 2;
}

void SPI_SendByte(unsigned char c)
{	
	SPTDAT1 = c; //发送数据会自动清零
	SPIStatus();  //注意，忙/闲判断一定要放在后面，否则会出问题！！！
}

/**/
unsigned char SPI_RevByte()
{
	SPTDAT1 = 0xff;
	SPIStatus();
	return SPRDAT1;
	
}



void ConfigSPI(int mode)
{
	GPIO_SPI_Init();
	SetClockMode(mode);
	SetPrescalerVal();
}








