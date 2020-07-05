#include "stdio.h"
#include "s3c24xx.h"
#include "int.h"
#include "lib/string.h"
#include "irq_num.h"
#include "timer.h"
#include "isr.h"
#include "lcd/lcd_controller.h"
#include "klist.h"
#include "i2c/i2c.h"
//#include "show_pic.h"
#include "spi/include/spi_flash.h"
#include "dma.h"
#include "nand.h"
#include "infrared/irda_raw.h"
#include "sound/sound.h"
#include "libmad/mad.h"
#include "lcd/picture/pic.h"
#include "lib/string.h"

extern int fatfs_app(void);

#define LCD_FB_BASE		0X33C00000
//#define IIC_INT
unsigned char *p[] ={gImage_pic};

void (*glob_func)(void);  /*全局函数指针*/
extern void irda_nec_test(void);

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
	init_interrupt();			/* 初始化中断控制器 */
	uart_init();				/* 初始化串口 */
	init_timer0();				/*初始化定时器0*/
	init_timer4(&timer);  		/*初始化定时器4*/
	ConfigSPI(0);				/* 配置SPI */
	InitSpiFlash();
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
	register_irq(INT_DMA0, DMA0_handler, 0);	/* 注册dma0中断 */
	register_irq(INT_DMA2, DMA2IntHandle, 0);	/* 注册dma2中断 */
	//register_irq(INT_RXD0, uart_recv_isr, 0);

	INIT_LIST_HEAD(&lcd_controller_list);	/* 初始化lcd_controller_list链表*/
	regist_s3c2440_lcd_controller();		/* 注册s3c2440 lcd控制器 */
	
	p_i2c_con = mach_i2c_controller("s3c440i2c");
	p_i2c_con->init();   // 初始化i2c控制器GPIO	

	lcd_init("s3c2440");	/* 初始化s3c2440 LCD */

	DMA0_Init();	/* 初始化DMA0 */
	Start_DMA();	/* 启动DMA0 */


	nand_read((unsigned char *)LCD_FB_BASE, 0x100000, 272*480*2);	/* 从nandflash 起始地址0x100000大小272*480*2的数据读到显存 */

	glob_func = led_blink;

	
	fatfs_app();	


#if 0
	spi_flash();
	play_sound();	/* 播放音频 */

	fatfs_app();


	//irda_raw_test();	/* 读取红外原始数据 */
	irda_nec_test();	/* 红外测试 */
	



	//i2c_app();	/* iic操作 */
#endif

	while(1) 
	{	
		glob_func();	
	}	

	
	return 0;
}


