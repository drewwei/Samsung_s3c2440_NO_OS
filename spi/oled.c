#include "ctl_spi.h"
#include "../../include/s3c24xx.h"
#include "oledfont.h"
#include "../../include/timer.h"
#include "../lib/string.h"

static void OLED_Set_CS(unsigned int val)
{
	if(0 == val) /*0 select*/
	{
		GPFDAT &= ~(1<<1);
	}
	else{
		GPFDAT |= (1<<1);
	}		
}
static void OLED_Set_DC(unsigned int val)
{
	if(0 == val) /*0:command*/
	{
		GPGDAT &= ~(1<<4);
	}
	else{
		GPGDAT = (1<<4); 
	}		
}

static void OledWriteCmd(unsigned char cmd)
{
	OLED_Set_CS(0);
	OLED_Set_DC(0);
	
	SPI_SendByte(cmd);
	
	OLED_Set_CS(1);
}
static void OledWriteData(unsigned char data)
{
	OLED_Set_CS(0);
	OLED_Set_DC(1);
	
	SPI_SendByte(data);
	
	OLED_Set_CS(1);
}

static void SetPosition(int page, int column)
{
	OledWriteCmd(0xB0 + page);
	OledWriteCmd(column&0x0f);
	OledWriteCmd(0x10 + (column >> 4));
}
static void ClearScreen()
{
	int i = 0, page = 0;
	while(page < 8)
	{
		OledWriteData(0);
		if(i > 127)
		{
			page++;
			SetPosition(page, 0);
			i = -1;
		}
		i++;
	}
}

void OLED_Init()
{
	OledWriteCmd(0xAE); /*display off*/
	OledWriteCmd(0x00); /*set lower column address*/
	OledWriteCmd(0x10); /*set higher column address*/
	OledWriteCmd(0x40); /*set display start line*/	
	OledWriteCmd(0xB0); /*set page address*/
	OledWriteCmd(0x81); /*contract control*/
	OledWriteCmd(0x66); /*128*/
	OledWriteCmd(0xA1); /*set segment remap*/
	OledWriteCmd(0xA6); /*normal / reverse*/
	OledWriteCmd(0xA8); /*multiplex ratio*/
	OledWriteCmd(0x3F); /*duty = 1/64*/
	OledWriteCmd(0xC8); /*Com scan direction*/
	OledWriteCmd(0xD3); /*set display offset*/
	OledWriteCmd(0x00);
	OledWriteCmd(0xD5); /*set osc division*/
	OledWriteCmd(0x80);
	OledWriteCmd(0xD9); /*set pre-charge period*/
	OledWriteCmd(0x1f);
	OledWriteCmd(0xDA); /*set COM pins*/
	OledWriteCmd(0x12);
	OledWriteCmd(0xdb); /*set vcomh*/
	OledWriteCmd(0x30);
	OledWriteCmd(0x8d); /*set charge pump enable*/
	OledWriteCmd(0x14);
	
	OledWriteCmd(0x20);/* Set Memory Addressing Mode (20h) */
	//OledWriteCmd(0x02);/*Page addressing mode (A[1:0]=10xb) */
	OledWriteCmd(0x00);/* Horizontal addressing mode (A[1:0]=00b) */
	ClearScreen(); /*清屏*/
	
	OledWriteCmd(0xAF); /*display ON*/	
	
	mdelay(100);
}

void OledPutChar(unsigned char c,unsigned char page, unsigned char column)
{
	int i;
	SetPosition(page, column);
	
	unsigned char *dots = oled_asc2_8x16[c - ' '];
	
	for(i = 0; i < 8; i++)
	{
		OledWriteData(dots[i]);
	}
	
	SetPosition(page + 1, column);
	
	for(i = 0; i < 8; i++)
	{
		OledWriteData(dots[i+8]);
	}
}

static void PutOneLine(unsigned char * pstr, unsigned char page, unsigned char column, int line)
{
	int i, j = 0, cnt = 0;
	unsigned char data;
	while(pstr[j])
	{	
		for(i = 0; i < 8; i++)
			{
				if(line == 0)
				{
					data = oled_asc2_8x16[pstr[j] - ' '][i]; /*从字模中获取字符*/
				}
				else
				{
					data = oled_asc2_8x16[pstr[j] - ' '][i+8];
				}
				OledWriteData(data);
			}	
		j++;
		cnt++;			
		if(cnt > 15) /* 一行最多写128/8 = 16 个字  0-15*/
		{
			page += 2;	   				/*一个字占两行16字节*/
			SetPosition(page, column);  /*重新定位, 因为页地址模式（page address mode），指针会自动回到行首，但不会换行*/			
			cnt = 0;        			/*cnt重新计数*/
		}
	}
}
void OLED_Print(unsigned char * str, unsigned char page, unsigned char column)
{
	

	SetPosition(page, column);
	/*先写完上半行，再换行写另外一行*/

	PutOneLine(str , page, column, 0);
	
	SetPosition(page + 1, column); /*换行*/
	
	PutOneLine(str , page + 1, column, 1);
	
}

static int PutOneLineHoriAddrMode(unsigned char * pstr, int line, int off, int column)
{
	int i, cnt = column;
	int j = off;
	unsigned char data;

	while(cnt < 127) //一行128个字节，写一个字节指针自动往后移一位
	{	
		if(pstr[j])
		{
			for(i = 0; i < 8; i++)
			{
				if(line == 0)
				{
					data = oled_asc2_8x16[pstr[j] - ' '][i]; /*从字模前8个字节获取字符*/	
				}else {
					data = oled_asc2_8x16[pstr[j] - ' '][i+8]; /*从字模后8个字节中获取字符*/
				}
								
				OledWriteData(data);
				cnt++;	//记录该行写的列位置
			}
			j++;       //记录字模数字的下标
		}
		else  //必须要做的动作，为了让指针移动到行首，必须要有写动作, Horizontal addressing mode 会自动换行回车
		{
			OledWriteData(0);  /*如果一行没有写满，则不显示,即写0*/
			cnt++;		//记录该行写的列位置
		}		
	}
	return j;    //返回字模数字的下标
	
}
void OLED_PrintHoriAddrMode(unsigned char * pstr)
{
	int offset = 0,ret;
	while(pstr[offset])
	{
		PutOneLineHoriAddrMode(pstr, 0, offset, 0); //逐行显示
		ret = PutOneLineHoriAddrMode(pstr, 1, offset, 0);
		offset = ret; //记录字模数字的位置
	}	
}
		

void OLED_PrintHoriAddrModeMove(unsigned char * pstr)
{
	int offset = 0;
	int i ;
	while(1)
	{
		
		if((offset >= strlen(pstr)) ||(offset == 0))
		{
			offset = 0;
			for(i = 119; i > 0; i = i-8) /*为了一开始让文字从右逐渐向左显示，先定位到屏幕最后一个字节的位置*/
			{				
				SetPosition(0,i);				
				PutOneLineHoriAddrMode(pstr, 0, offset, i);
				SetPosition(1,i);
				PutOneLineHoriAddrMode(pstr, 1, offset, i);	
				mdelay(100);
			}
			
		}
		else{
			SetPosition(0, 0);
			PutOneLineHoriAddrMode(pstr, 0, offset, 0);
			PutOneLineHoriAddrMode(pstr, 1, offset, 0);
		}		
		offset++;			
		mdelay(100);
	}	
}