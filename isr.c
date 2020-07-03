#include "s3c24xx.h"
#include "string.h"
#include "int.h"
#include "timer.h"
#include "show_pic.h"
#include "dma.h"
#define MAX		3
#define LCD_FB_BASE		0X33C00000
volatile int offset;
extern volatile int flag;
void i2c_handler_1(void)
{
	flag = 0;
}
void timer4_func(void) 
{
	uart_send("this is timer4_func\r\n");
	if(!(GPFDAT&1))
	{
		GPFDAT |= (0x7<<4);   // 所有LED熄灭
		GPFDAT &=  ~(1<<6);	
	
		if(offset< (MAX-1))
		{
			show_pic_16(p[++offset]);
		}
		else if(offset == (MAX-1))
		{
			offset = 0;
			show_pic_16(p[offset]);
		}
	}	
}
extern struct timer_list timer;
void eint0_handler(void)      //外部中断0中断服务函数isr
{ 	
	mod_timer(&timer, 10);	/*修改超时时间，消抖，定时器延时10ms,超时后在定时器中断中判断按键是否真的被按下,经测试10ms正好可以消抖*/
	Start_DMA();
}
/************以下中断服务函数均为测试用****************/

void eint2_handler(void)
{ 
	mdelay(10);
	if(!(GPFDAT&(1<<2)))
	{
		GPFDAT  |= (0x7<<4);  
		GPFDAT  &= ~(1<<5);
	
		if(offset > 0)
		{
		show_pic_16(p[--offset]);
		}
		else if(offset == 0)
		{
		offset = MAX-1;
		show_pic_16(p[offset]);
		}
	}	
}
/*从spiflash地址处读到现存*/
extern void spi_flash(void);
void eint11_handler(void)
{ 
	uart_send("--------I am eint11_handler-----------\r\n");
	spi_flash();
}
void eint19_handler(void)
{ 
	int i = 3;
	while(i--){
		GPFDAT  |= (0x7<<4);
		
		GPFDAT  &= ~(1<<4);
		uart_send("--------I am eint19_handler-----------\r\n");
		mdelay(500);
		GPFDAT  |= (1<<4);
		mdelay(500);
	}
}

void switch_pic(void)
{
	static int j = 0;
	if(offset == MAX) offset = 0;
	if(j%2 == 0) GPFDAT  &= ~(1<<5);
	else	 	 GPFDAT  |=  (1<<5);	
	j++;
	show_pic_16(p[offset++]);
	mdelay(1000);	
}


char a[60];
extern void (*glob_func)(void);
extern void spi_flash(void);
void uart_recv_handler(void)
{
	/*
	unsigned char a[30];
	a[29] = '\0';
	int i = 0;
	recv_Ready();
	a[i] = URXH0;
	i++;
	uart_send(a);	
	*/	
/*
	unsigned char a;
	recv_Ready();
	a = URXH0;
	putc(a);
	*/
/*
	static int i = 0;
	recv_Ready();
	a[i] = URXH0;
	i++;
	if(i >= 58)
		i = 0;	
	*/
	int i = 0;
	while(UFSTAT0 & 0x3f)  //fifo不空就读
	{
		a[i] = URXH0;
		i++;
	}
	a[i]='\0';
	uart_send(a);
	if(!strcmp(a,"AT+SWPIC=1"))
	{
		glob_func = switch_pic;
	}
	else if(!strcmp(a,"AT+SWPIC=0")) {	
		glob_func = led_blink;
	}
	else if(!strcmp(a,"AT+SWPIC=2")) {	
		glob_func = spi_flash;
	}
}


