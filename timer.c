#include "stdio.h"
#include "s3c24xx.h"
#include "irq_num.h"
#include "int.h"
#include "sound/sound.h"
#include "lib/string.h"
/*Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value} = 50M/5/2=5M ; cycle = 0.2us*/

static unsigned long long g_system_time_10ms_cnt = 0;
static void start_timer0(void)
{
	TCON |= (1 << 0);
}
static void stop_timer0(void)
{
	TCON &= ~(1 << 0);
}

static void manual_update_timer0(void)
{
	TCON |= (1 << 1);
}
static void clear_manual_update_timer0(void)
{
	TCON &= ~(1 << 1);
}

static void auto_reload_timer0(void)
{
	TCON |= (1 << 3);
}
static void cfg_init_timer0(void)
{
	TCFG0 &=~(0xff << 0);  //Prescaler1 =4
	TCFG0 |= (4 << 0);
	TCFG1 &= ~(0xf << 0); //divider =1/2
	
	CLKCON |= (1<<8);   //使能PWMTIMER时钟
}
static void manual_update_timer4(void)
{
	TCON |= (1 << 21);
}
static void clear_manual_update_timer4(void)
{
	TCON &= ~(1 << 21);
}

static void stop_timer4(void)
{
	TCON &= ~(1 << 20);
}
void start_timer4(void)
{
	TCON |= (1 << 20);
}
static void cfg_init_timer4(void)
{
	//TCFG0 &=~(0xff << 8);  
	TCFG0 |= (0xff << 8); //Prescaler1 =255
	TCFG1 &= ~(0xf << 16); 
	TCFG1 |= (3 << 16); //divider =1/16
}
/*timer0 用来延迟*/
void init_timer0(void)
{
	stop_timer0();
	cfg_init_timer0();
	TCNTB0 = 50000;
	manual_update_timer0();	
	clear_manual_update_timer0(); //注意：手动更新位必须在更新TCNTB后清除！！！	
	auto_reload_timer0();
	start_timer0();
}

/*timer4 用来定时*/
void init_timer4(struct timer_list *timer)
{
	unsigned cnt = (timer->expire) * 12;
	if(cnt>65535)
		cnt = 65535;
	stop_timer4();
	cfg_init_timer4(); //初始化定时器4
	TCNTB4 = cnt;   //设置超时时间
	manual_update_timer4();//手动更新
	clear_manual_update_timer4(); //注意：手动更新位必须在更新TCNTB后清除！！！
	register_irq(INT_TIMER4, timer->function, 0); //设置中断服务函数
	//start_timer4();   //启动定时器
}
void mod_timer(struct timer_list *timer, unsigned int msec)
{
	stop_timer4();
	unsigned cnt = msec * 12;
	if(cnt>65535)
		cnt = 65535;
	TCNTB4 = cnt;   //设置超时时间
	manual_update_timer4();//手动更新
	clear_manual_update_timer4(); //注意：手动更新位必须在更新TCNTB后清除！！！
	start_timer4();   //启动定时器
}
void udelay(unsigned int time)
{
	volatile int count;
	volatile int prev,cur;
	count = time*5;  //0.2us*5=1us
	prev = TCNTO0;
	
	while(count > 0)
	{
		cur = TCNTO0;
		//printf("udelay cout: %d cur:%d\r\n", count, cur);
		if(cur < prev)	
		{
			count -= (prev - cur);
		}
		else{
			count -= (50000 + prev -cur);
		}		
		prev = cur;
	}
}

void mdelay(unsigned int time)
{
	udelay(1000*time);
}

void timer0_handler(void)
{	
	g_system_time_10ms_cnt++;
	
}
unsigned long long get_system_time_us(void)
{
	unsigned long long us = (50000 - TCNTO0)/5;
	return g_system_time_10ms_cnt*10*1000 + us;
}

unsigned int how_long_time_us(unsigned long long pre, unsigned long long now)
{
	return now-pre;
}



















































