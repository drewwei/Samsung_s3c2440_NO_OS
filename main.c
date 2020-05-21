#include "stdio.h"
#include "s3c24xx.h"
#include "int.h"
#include "lib/string.h"
#include "irq_num.h"
#include "timer.h"
#include "isr.h"
#include "lcd/include/lcd_controller.h"
#include "klist.h"
#include "i2c/i2c.h"
#include "i2c/s3c2440_i2c.h"
#include "i2c/i2c_core.h"
#include "i2c/at24cxx.h"
#include "show_pic.h"
#include "spi/include/ctl_spi.h"
#include "dma.h"
#include "nand.h"
#include "NEC/irda_raw.h"
#define LCD_FB_BASE		0X33C00000
//#define IIC_INT
unsigned char data[480*272];
unsigned char *p[] ={data};

void (*glob_func)(void);  /*全局函数指针*/
extern void irda_nec_test(void);
void dump(void)
{
		GPFDAT  &= ~(1<<5);
		mdelay(500);
		GPFDAT  |= (1<<5);
		mdelay(500);
}


struct timer_list timer = {
		.expire   = 200,			/*超时时间200ms,给个初始值，后面会改*/
		.function = timer4_func,	/*中断服务函数*/
	};
struct i2c_controller s3c2440_i2c_controller;  /*定义一个IIC主机控制器结构体变量*/

struct list_head lcd_controller_list;		/*定义一个链表头*/

int main(void)
{
	int ret;
	struct i2c_controller *p_i2c_con;
	init_gpio();
	init_interrupt();
	uart_init();
	init_timer0();
	ConfigSPI(0);
	init_s3c2440_i2c_controller(&s3c2440_i2c_controller);   /*初始化s3c2440 IIC主机控制器结构体*/
	ret = register_i2c_controller(&s3c2440_i2c_controller); /*注册IIC 主机控制器*/
	if(ret == -1)
		uart_send("error:register_i2c_controller fail.\r\n");
	
#ifdef IIC_INT
	register_irq(INT_IIC, i2c_handler, 0);
#else
	register_irq(INT_IIC, i2c_handler_1, 0);
#endif
	register_irq(EINT0, eint0_handler, Falling_edge_triggered);
	register_irq(EINT2, eint2_handler, Falling_edge_triggered);
	register_irq(EINT11, eint11_handler, Falling_edge_triggered);
	register_irq(EINT19, eint19_handler, Both_edge_triggered);
	register_irq(INT_TIMER0, timer0_handler, 0);
	register_irq(EINT1, eint1_handler, Both_edge_triggered);
	//register_irq(INT_RXD0, uart_recv_isr, 0);

	GPFDAT &= ~((1<<4)|(1<<5)|(1<<6));  /*输出低电平*/
	init_timer4(&timer);  				/*初始化定时器4*/
	
	INIT_LIST_HEAD(&lcd_controller_list);	/*初始化该链表*/
	
	p_i2c_con = mach_i2c_controller("s3c440i2c");
	p_i2c_con->init();   // 初始化i2c控制器GPIO	
	lcd_init();

	DMA0_Init();
	Start_DMA();

	nand_read(LCD_FB_BASE, 0x100000, 272*480*2);
	//show_pic_16(data);

	glob_func = dump;

	//irda_raw_test();
	irda_nec_test();
	
	while(1) 
	{	
		glob_func();	
	}


	unsigned char data0,data1 = 7,c;
	while(1)
	{
		uart_send("please enter 'r' or 'w' for read and write\r\n");
		c = getc();
		switch(c)
		{
			case 'w':
				uart_send("please enter a char.\r\n");
				data0 = getc();
#ifdef IIC_INT
				AT24CXX_i2c_write_OneByte(1,data0);
#else
				AT24CXX_i2c_write(1, &data0, 1);
#endif
				break;
			case 'r':
#ifdef IIC_INT
				AT24CXX_i2c_read_OneByte(1,&data1);
#else
				AT24CXX_i2c_read(1,&data1, 1);
#endif
				printf("data1:%d\r\n",data1);
				break;
			default:
				break;
		}	
	
	}


		

		
	return 0;
}
