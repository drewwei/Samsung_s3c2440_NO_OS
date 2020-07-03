#include "ctl_spi.h"
#include "../include/s3c24xx.h"
#include "../include/timer.h"
#include "../include/stdio.h"

#define PAGELEN				256
#define BLOCKSIZE			(64*1024)
#define SECTORSIZE			1024*4

/*flash片选*/
static void SPI_FlashCS(int select)
{
	if(select == 0) 
	{
		GPGDAT |= (1<<2); /*为高电平时取消选择*/
	}else
	{
		GPGDAT &= ~(1<<2);
	}
}
/*发地址*/
static void SendAddress(unsigned int addr)
{
	SPI_SendByte((addr>>16)&0xff);
	SPI_SendByte((addr>>8)&0xff);
	SPI_SendByte(addr&0xff);
}
/*读ID*/
void ReadID(unsigned char *PMID, unsigned char *PDID)
{
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x90); //发命令
	SendAddress(0);
	*PMID = SPI_RevByte();
	*PDID = SPI_RevByte();
	
	SPI_FlashCS(0);	
}



/*写使能*/
static void WriteEnable()
{
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x06);
	SPI_FlashCS(0);	//取消片选
}
/*写禁止*/
static void WriteDisable()
{
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x04);
	SPI_FlashCS(0);	//取消片选
}
/* 写状态寄存器是能 */
static void WriteEnableForStatusReg()
{
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x50);
	SPI_FlashCS(0);	//取消片选
}
/* 读状态寄存器1 */
static unsigned char ReadStatusReg1()
{
	unsigned char ret;
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x05);	
	ret = SPI_RevByte();
	
	SPI_FlashCS(0);	//取消片选
	return ret;
}
/* 读状态寄存器2 */
static unsigned char ReadStatusReg2()
{
	unsigned char ret;
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x35);	
	ret = SPI_RevByte();
	
	SPI_FlashCS(0);	//取消片选
	return ret;
}
/* 写状态寄存器 */
static void WriteStatusReg(unsigned char reg1, unsigned char reg2)
{
	WriteEnableForStatusReg(); //写使能
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x01);
	SPI_SendByte(reg1);
	SPI_SendByte(reg2);	
	SPI_FlashCS(0);	//取消片选

}

/*判断忙*/
static int BusyStatus()
{
	int count = 0xfffff;
	while(ReadStatusReg1()&1)
	{
		if(!count) return -1;
		count--;
	}
	return 0;
}

/*擦除整个芯片*/
void ChipErase()
{
	WriteEnable(); //写使能
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0xc7);
	SPI_FlashCS(0);	//取消片选
	BusyStatus();
}


/*扇区擦除,addr为扇区地址*/
void SectorErase(unsigned int sector)
{
	WriteEnable(); //写使能
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x20);
	SendAddress(sector); //发地址
	SPI_FlashCS(0);	//取消片选

	BusyStatus();
}
/*块擦除,addr为块地址*/
void BlockErase(unsigned int block)
{
	WriteEnable();  //写使能
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0xd8);
	SendAddress(block); //发地址
	SPI_FlashCS(0);	//取消片选

	BusyStatus();
}


static void SPIFlashClearProtectForStatusReg(void)
{
    unsigned char reg1, reg2;

    reg1 = ReadStatusReg1();
    reg2 = ReadStatusReg2();

    reg1 &= ~(1<<7);
    reg2 &= ~(1<<0);

    WriteEnableForStatusReg();
	
	WriteStatusReg(reg1, reg2);
}
static void SPIFlashClearProtectForData(void)
{
    /* cmp=0,bp2,1,0=0b000 */
    unsigned char reg1, reg2;

    reg1 = ReadStatusReg1();
    reg2 = ReadStatusReg2();

    reg1 &= ~(7<<2);
    reg2 &= ~(1<<6);

    WriteEnableForStatusReg();
	
	WriteStatusReg(reg1, reg2);
}

static void Unprotect()
{
	int ret = BusyStatus();
	if(ret) 
	{
		printf("Error,BusyStatus timeout\r\n");
		return;
	}
	unsigned char reg1 = ReadStatusReg1();
	unsigned char reg2 = ReadStatusReg2();
	reg1 &= ~((1 << 7) | (7 << 2)); 
	//reg1 &= ~(1 << 7); //也可以
	reg2 &= ~(1<<0); 

	WriteEnableForStatusReg();
	
	WriteStatusReg(reg1, reg2);
}


/*页编程*/
void  PageProgram(unsigned int addr, unsigned char *data, unsigned int size)
{
	int i, ret;
	WriteEnable();   //写是能
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x02);  //发指令
	SendAddress(addr); //发地址
	for(i = 0; i < size; i++) 
	{		
		SPI_SendByte(data[i]);  //发数据			
	}
	
	SPI_FlashCS(0);	//取消片选
	ret = BusyStatus();
	if(ret) 
	{
		printf("Error,PageProgram BusyStatus timeout\r\n");
	}//循环判断忙
}
/*扇区编程*/
int SectorProgram(unsigned int addr, unsigned char *data)
{
	int i;
	int secoff = addr%SECTORSIZE;
	if(secoff != 0) return -1;	
	for(i = 0; i < (SECTORSIZE/PAGELEN); i++)
	{
		PageProgram(addr + i*PAGELEN, &data[PAGELEN*i], PAGELEN);
	}
	return 0;
}

/*读数据*/
void SPIReadData(unsigned int addr, unsigned char *data, int size)
{
	int i = 0;
	int ret = BusyStatus();
	if(ret) 
	{
		printf("Error,BusyStatus timeout\r\n");
		return;
	} //循环判断忙
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x03);  //发指令
	SendAddress(addr); //发地址
	
	for(i = 0; i < size; i++)
	{
		data[i] = SPI_RevByte();
	}
	
	SPI_FlashCS(0);	//取消片选

}

/* addr：写入的flash地址
 * data：写入flash的内容
 * size: 写入的大小
 * PAGELEN: 宏，一个page的大小
 * pageremain: 一页可写的字节数，小于等于PAGELEN
 * CurWriteSize: 当前页写入的字节大小,可能等于PAGELEN，也可能小于PAGELEN
 * addr%PAGELEN: 从该页的第几列开始写
 */
unsigned char buff[SECTORSIZE];
void SPIWriteData(unsigned int addr, const unsigned char *data, int size)
{
	int i;
	int sectremain;
	int dataremain;
	int count;
	int lastsectremain;
	//int pageremain, curwritesize;
	int sect  = addr/SECTORSIZE;
	int secoff = addr%SECTORSIZE;
	
	Unprotect(); //注意：擦除之前必需要去保护！！！
	//BlockErase(blcok); //先确定擦除块！
	//SectorErase(sect);//再确定擦除哪页！	

	SPIReadData(sect*SECTORSIZE, buff, SECTORSIZE);	/* 先读取要写的扇区 */

	SectorErase(sect); /* 然后擦除该扇区 */

	sectremain = sect - secoff; 

	if(size <= sectremain)
	{
		for(i = secoff; i < secoff+size; i++)
		{
			buff[i] = data[i-secoff];
		}
		SectorProgram(sect*SECTORSIZE, buff);
	}else
	{
		for(i = secoff; i < SECTORSIZE; i++)
		{
			buff[i] = data[i-secoff];
		}
		SectorProgram(sect*SECTORSIZE, buff);

		dataremain = size -sectremain;
		count = (dataremain-1)/SECTORSIZE + 1;	/* 剩下的数据占用多少个扇区 */

		lastsectremain = dataremain - (count-1)*SECTORSIZE;	/* 计算最后一个扇区占用多少字节 */
		/* 中间的N个扇区不需要考虑是否读出数据 */
		i = 0;
		while(--count)
		{
			sect++;
			SectorErase(sect);//擦除哪个扇区！
			SectorProgram(sect*SECTORSIZE, data[sectremain + SECTORSIZE*i]);	/* 从数组第sectremain个数据开始写入*/
			i++;
		}
		sect++;
		SPIReadData(sect*SECTORSIZE, buff, SECTORSIZE);	/* 从最后一个要写的扇区读出数据 */
		SectorErase(sect);	/* 然后擦除该扇区 */
		for(i = 0; i <lastsectremain; i++ )
		{
			buff[i] = data[size - lastsectremain + i];
		}
		SectorProgram(sect*SECTORSIZE, buff);
	}
	
#if 0
	while(size > 0)
	{			
		pageremain = PAGELEN - addr%PAGELEN; //pageremain一页可写的字节数,当addr+CurWriteSize后%PAGELEN必然为0;
		if(size <= pageremain) //如果写入的数据少于一页中可写的个数
		{
			curwritesize = size;   //size为本次写入的字节数
		}		
		else{
			curwritesize = pageremain; //当col一开始不为0时,第一次循环pageremain值为PAGELEN - addr%PAGELEN，第二次循环时必然为PAGELEN，addr%PAGELEN=0;			
		}
		PageProgram(addr, data, curwritesize);//写数据
		addr += curwritesize;
		data += curwritesize;
		size -= curwritesize;

	}
#endif
}


//会绕道页头覆盖之前写的东西.
/*
void WriteData(unsigned int addr, unsigned char *data, int size)
{
	int i,block;
	block = addr/64/1024;
	while(BusyStatus()); //循环判断忙
	BlockErase(block); //擦除块	

	while(BusyStatus()); //循环判断忙
	WriteEnable();   //写是能
	SPI_FlashCS(1);   //选中芯片
	SPI_SendByte(0x02);  //发指令
	SendAddress(addr); //发地址
	for(i = 0; i < size; i++) 
	{
		SPI_SendByte(*pdata);  //发数据
		pdata++;
	}
	SPI_FlashCS(0);	//取消片选
}
*/

void InitSpiFlash()
{
	SPIFlashClearProtectForStatusReg();
	SPIFlashClearProtectForData();
	//while(BusyStatus()); //循环判断忙
}


