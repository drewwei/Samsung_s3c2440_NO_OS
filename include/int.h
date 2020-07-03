#ifndef __INIT__
#define __INIT__


extern void init_gpio(void);

extern void uart_init(void);
extern void uart_send(char * data);

extern char *itoc(const int value);//整形转字符串

extern int register_irq(unsigned int offset, void(*handler)(void), unsigned int trigger);
extern void init_interrupt(void);

extern void iic_init(void);
extern void led_blink(void);

struct timer_list{
	unsigned int expire;
	void(*function)(void);
 };


#endif


