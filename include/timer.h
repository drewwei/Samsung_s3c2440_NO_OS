#ifndef __TIMER__
#define __TIMER__


#include "int.h"

extern void init_timer4(struct timer_list *timer);
extern void start_timer4(void);
extern void mod_timer(struct timer_list *timer, unsigned int msec);

extern void init_timer0(void);


extern void udelay(unsigned int time);
extern void mdelay(unsigned int time);

extern void timer4_func(void);

extern unsigned int how_long_time_us(unsigned long long pre, unsigned long long now);
extern unsigned long long get_system_time_us(void);

#endif