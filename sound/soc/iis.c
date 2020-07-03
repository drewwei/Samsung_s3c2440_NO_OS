#include "../../include/stdio.h"

#define rIISCON 	(*(volatile unsigned int *)0x55000000)
#define rIISMOD 	(*(volatile unsigned int *)0x55000004)
#define rIISPSR		(*(volatile unsigned int *)0x55000008)
#define rIISFCON	(*(volatile unsigned int *)0x5500000C)
#define rIISFIFO	(*(volatile unsigned int *)0x55000010)
#define GPECON    	(*(volatile unsigned int *)0x56000040)
#define PCLK 50000000

#define ABS(a, b)	((a)-(b))>0?((a)-(b)):((b)-(a))

/* bit[9] : Master clock select, 0-PCLK
	* bit[8] : 0 = Master mode
	* bit[7:6] : 10 = Transmit mode
	* bit[4] : 0-IIS compatible format
	* bit[2] : 384fs, 确定了MASTER CLOCK之后, fs = MASTER CLOCK/384
	* bit[1:0] : Serial bit clock frequency select, 32fs
	*/
//初始化iis主机控制器
void iis_init(int bits_per_sample, int fs)
{
	int tmp_fs = 0,min = 0xfffff;
	int pre = 0,i;
	
	 /* 配置GPIO用于IIS */
    GPECON &= ~((3<<0) | (3<<2) | (3<<4) | (3<<6) | (3<<8));
    GPECON |= ((2<<0) | (2<<2) | (2<<4) | (2<<6) | (2<<8));
	/*
     * bit[5] : Transmit DMA service request, 1-enable
     * bit[1] : IIS prescaler, 1-enable
     */
	rIISCON = (1 << 5) | (1 << 1);

	if(bits_per_sample == 8)
		rIISMOD = (2 << 6) | (1 << 2) | (1);
	else if(bits_per_sample == 16)
		rIISMOD = (2 << 6)| (1 << 3) | (1 << 2) | (1);
	
	/* Master clock = PCLK/(n+1)
     * fs = Master clock / 384
     * fs = PCLK / (n+1) / 384
     */

	printf("Master clock\r\n");
	for (i = 0; i < 32; i++)
	{
		tmp_fs = PCLK/384/(i+1);
		if (ABS(tmp_fs,fs) < min)
			{
				min = ABS(tmp_fs,fs);
				pre = i;	
			}
	}	
	printf("fs:%d; pre:%d\r\n", PCLK/384/(pre+1), pre);

	rIISPSR = (pre << 5) | (pre);
	 /*
     * bit15 : Transmit FIFO access mode select, 1-DMA
     * bit13 : Transmit FIFO, 1-enable
     */
	rIISFCON = (1 << 15) | (1 << 13) ;
	
}

void start_iis()
{
	rIISCON |= (1);
}

void stop_iis()
{
	rIISCON &= ~1;
}

































