#include "s3c24xx.h"
#include "irq_num.h"

extern void uart_recv_int(void);
extern void uart_send(unsigned char * data);
static void init_isr(void);



typedef struct irq_handler_struct{
	unsigned int flag;           //0表示未注册；1表示已经注册
	void(*action)(void);		// 中断处理函数isr
	void(*init)(unsigned int, unsigned int trigger); //中断初始化
	//void (*exit)(unsigned int);
}irq_handler_t;

irq_handler_t irq_desc[IRQ_LEN];  //该数组存放了以中断号位下表的irq_handler_t结构体，在调用init_interrupt函数后被初始化

void init_interrupt(void)
{
	INTMOD = 0x0; 	/* IRQ模式*/
	PRIORITY = 0x0;	/* 固定优先级*/
	
#if 0
	INTMSK &= ~((1<<0)|(1<<2)|(1<<5));  										/* 使能eint0,eint2,eint8-23 */
	EINTMASK &= ~((1<<11) | (1<<19));											/* 外部中断(子中断)使能 eint11,eint19*/
	EXTINT0 &= ~((3<<0) | (3<<8));
	EXTINT0 |= (Falling_edge_triggered << 0) | (Falling_edge_triggered << 8);								/* 设置中断低电平触发方式*/
	EXTINT1 &= ~(3 << 12);
	EXTINT1 |= (Falling_edge_triggered<<12);
	
	INTMSK 	  &= ~(1<<28);     													/* 中断屏蔽控制器不屏蔽uart0中断*/
	INTSUBMSK &= ~(1<<0);														/*子中断控制器不屏蔽INT_RXD0中断*/		
#endif
	init_isr();
	
	__asm__("msr cpsr_c, #0x53\n");		//开启cpsr中断位.arm复位后自动关闭I F位
}


static void template_isr(void)
{

}
static void template_init(unsigned int irq_num, unsigned int trigger)
{
	if(irq_num < 32)
		INTMSK &= ~(1<<irq_num);	
}
static void eint_distribute(void)
{
	
	int suboff = EINTPEND;

	int i;
	for(i = 4; i <= 23; i++)
	{
		if(suboff&(1<<i))
			break;
	}
	
	(irq_desc[i+32].action)();
	
	EINTPEND = (1<<i); 				/*清理子中断*/
}

static void sbu_interrupt_distribute(void)
{
	int suboff = SUBSRCPND;	
	int i;
	for(i = 0; i <= 14; i++)
	{
		if(suboff&(1<<i))
			break;
	}
	(irq_desc[i+56].action)();
	SUBSRCPND = (1<<i);   /*清理子中断*/
}

static void eint0_3_init(unsigned int irq_num, unsigned int trigger)
{
	GPFCON 	&=~(3<<irq_num*2);
	GPFCON 	|= (2<<irq_num*2);
	INTMSK 	&=~(1<<irq_num);
	EXTINT0 &=~(7<<irq_num*4);
	EXTINT0 |= (trigger<<irq_num*4);
}
static void eint4_7_init(unsigned int irq_num, unsigned int trigger)
{
	GPFCON   &=~(3<<((irq_num - 32)*2));
	GPFCON 	 |= (2<<((irq_num - 32)*2));
	INTMSK 	 &=~(1<<4);
	EINTMASK &=~(1<<(irq_num - 32));
	EXTINT0  &=~(7<<((irq_num - 32)*4));
	EXTINT0  |= (trigger<<((irq_num - 32)*4));
}
static void eint8_23_init(unsigned int irq_num, unsigned int trigger)
{
	GPGCON 	 &=~(3<<((irq_num - EINT8)*2));
	GPGCON 	 |= (2<<((irq_num - EINT8)*2));
	INTMSK 	 &=~(1<<5);
	EINTMASK &=~(1<<(irq_num - 32));
	if((irq_num >= EINT8)&&(irq_num <= EINT15))
	{
		EXTINT1 &=~(7<<(irq_num - EINT8)*4);
		EXTINT1 |= (trigger<<((irq_num - EINT8)*4));
	}
	else{
		EXTINT2 &=~(7<<(irq_num - EINT16)*4);
		EXTINT2 |= (trigger<<((irq_num - EINT16)*4));
	}
}
static void INT_RXD0_init(unsigned int irq_num, unsigned int trigger)
{
	INTMSK &= ~(1<<28);	
	INTSUBMSK &= ~(1<<0);
}
static void INT_RXD1_init(unsigned int irq_num, unsigned int trigger)
{
	INTMSK &= ~(1<<28);	
	INTSUBMSK &= ~(1<<1);
}
static void init_isr(void)
{
	int i;
	for(i = 0; i < IRQ_LEN; i++)
	{
		irq_desc[i].action = template_isr;
		irq_desc[i].init = template_init;
	}	
	for(i = EINT4_7; i <= EINT8_23; i++)
	{
		irq_desc[i].action = eint_distribute;
	}

	irq_desc[28].action = sbu_interrupt_distribute;
	irq_desc[23].action = sbu_interrupt_distribute;
	irq_desc[15].action = sbu_interrupt_distribute;
	irq_desc[31].action = sbu_interrupt_distribute;
	irq_desc[9].action = sbu_interrupt_distribute;
	irq_desc[6].action = sbu_interrupt_distribute;
	
	for(i = EINT0; i <= EINT3; i++)
	{
		irq_desc[i].init = eint0_3_init;
	}
	
	for(i = EINT4; i <= EINT7; i++)
	{
		irq_desc[i].init = eint4_7_init;
	}
	for(i = EINT8; i<=EINT23; i++)
	{
		irq_desc[i].init = eint8_23_init;
	}
	irq_desc[INT_RXD0].init = INT_RXD0_init;
	irq_desc[INT_RXD1].init = INT_RXD1_init;
}

/* offset 是我在irq_num.h中定义的中断号，回调函数handler是真正的中断处理函数 ，trigger表示中断触发方式*/
int register_irq(unsigned int offset, void(*handler)(void), unsigned int trigger)  
{
	unsigned int t = irq_desc[offset].flag;
	if(t)
		return -1;
	irq_desc[offset].action = handler;
	irq_desc[offset].init(offset, trigger);
	irq_desc[offset].flag = 1;	
	return 0;
}
/*中断发生时保存现场后调用的C语言函数*/
void IrqHandler(void)
{
	int offset = INTOFFSET;
	
	irq_desc[offset].action();         	/*调用中断服务函数*/
	SRCPND = (1<<offset); 				/* 需要在清理子中断后立即** 清上一级中断*/
	INTPND = (1<<offset);
}














/* 定义了两个全局数组isr[32]和eint_isr[23]，根据硬件中断号，将其相应的中断服务函数绑定给以中断号为下标的isr[]数组项，特别的：Isr[5]存放eint8-11的分发函数。
 * 外部中断8-23的中断服务函数存放在以外部中断号为下标的eint_isr[]数组项里，
 * 当发生eint11中断时，isr[5]会被调用，在初始化时isr[5]被赋值为distribute函数，该函数负责判断区分eint8-11哪个中断的发生，然后调用eint_isr[]相应项，处理完后，清理外部中断挂起，和中断源挂起、中断挂起。
 */

#if 0

void(*isr[32])(void);
void(*eint_isr[24])(void);
void(*sub_isr[15])(void);



static void template_isr(void)
{
}

extern unsigned char a[60];
static void eint0(void)
{ 
		GPFDAT  |=  (1<<4)|(1<<5)|(1<<6);
		GPFDAT  &=  ~(1<<4);
		uart_send(a);
		
}
static void eint2(void)
{ 
		GPFDAT  |= (0x7<<4);   // 所有LED熄灭
		GPFDAT  &= ~(1<<5);
}
static void eint11(void)
{ 	
		GPFDAT |= (0x7<<4);   // 所有LED熄灭
		GPFDAT  &=  ~(1<<6);
		
}
static void eint19(void)
{ 	
		//GPFDAT |= (0x7<<4);   // 所有LED熄灭
		GPFDAT  &= ~((1<<4)|(1<<5)|(1<<6));
}



static void eint_distribute(void)
{
	
	int suboff = EINTPEND;

	int i;
	for(i = 4; i <= 23; i++)
	{
		if(suboff&(1<<i))
			break;
	}
	(eint_isr[i])();
	EINTPEND = (1<<i); 				/*清理子中断*/
	
	SRCPND = (1<<5); 				/* 需要在清理子中断后立即** 清上一级中断*/
	INTPND = (1<<5);
}

void sbu_interrupt_distribute(void)
{
	int suboff = SUBSRCPND;	
	int i;
	for(i = 0; i <= 14; i++)
	{
		if(suboff&(1<<i))
			break;
	}
	(sub_isr[i])();
	SUBSRCPND = 1<<i;
	SRCPND = (1<<28); 				/* 需要在清理子中断后立即** 清上一级中断*/
	INTPND = (1<<28);
}

static void init_eint_isr(void)
{
	int i;
	for(i = 4; i <= 23; i++)
	{
		eint_isr[i] = template_isr;
	}
}
static void init_sub_isr(void)
{
	int i;
	for(i = 0; i <= 14; i++)
	{
		sub_isr[i] = template_isr;
	}
}
static void init_isr(void)
{
	int i;
	for(i = 0; i < 32; i++)
	{
		isr[i] = template_isr;
	}	
	isr[5] = eint_distribute;   		/* 将分发函数赋值给中断号为5的isr */
	isr[28]= sbu_interrupt_distribute;
}
static void bind_isr(unsigned int offset, void(*handler)(void))
{
	isr[offset] = handler;
}
static void bind_eint_isr(unsigned int offset, void(*handler)(void))
{
	eint_isr[offset] = handler;
}
static void bind_sub_isr(unsigned int offset, void(*handler)(void))
{
	sub_isr[offset] = handler;
}

static void bind_all_isr()
{
	bind_isr(0, eint0);
	bind_isr(2, eint2);
	bind_eint_isr(11, eint11);
	bind_eint_isr(19, eint19);	
	bind_sub_isr(0, uart_recv_int);
}

void cfg_interrupt(void)
{
	init_interrupt();
	init_isr();
	init_eint_isr();
	init_sub_isr();
	bind_all_isr();
		
	__asm__("msr cpsr_c, #0x53\n");		//开启cpsr中断位.arm复位后自动关闭I F位
}


void IrqHandler(void)
{	
	int offset = INTOFFSET;
	
	(isr[offset])();         	/*调用中断服务函数*/
	if((offset != 5) && (offset != 28))				/*外部中断eint8-23,uart0在分发函数里清除 */
	{
		SRCPND = (1<<offset); 		/*清中断*/
		INTPND = (1<<offset);
	}	
}


#endif








