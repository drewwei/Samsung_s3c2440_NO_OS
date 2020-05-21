#include "s3c24xx.h"
#include "int.h"
void uart_init(void)
{
	ULCON0 = 0x3;     /* 普通模式，无校验位，1个停止位，8个数据位 */
	//UFCON0	= 0x0;
	UMCON0 = 0x0;
	UBRDIV0 = 0x1A;  /*波特率115200*/
	UCON0  = 0x85;
	UFCON0 = 0x77;

	UCON0 &= ~(3<<0);
	UCON0 |= 2;			//使能接收DMA模式
}
static void send_Ready(void)
{
	while(!(UTRSTAT0 & (1<<1)));
}
static void recv_Ready(void)
{
	while(!(UTRSTAT0 & (1<<0)));
}

void putc(unsigned char c)
{
	while((UFSTAT0& (1<<14))); //fifo不满就写
	UTXH0 = c;
}
unsigned char getc(void)
{
	while(!(UFSTAT0 & 0x3f));  //fifo为空就就一直等;
	return URXH0;
}

void uart_send(char * data)
{
	while(*data)
	{
		//send_Ready();		
		while((UFSTAT0& (1<<14))); //fifo不满就写
		UTXH0 = *data;
		data++;				
	}	
}















