#include "irda_raw.h"
#include "../include/s3c24xx.h"
#include "../include/stdio.h"
#include "../include/timer.h"
/*IRDA 引脚： EINT1/GPF1*/

unsigned long long g_last_time = 0;
/* 获取红外数据：引脚极性 */
static int irda_data_get(void)
{
    /* 获取引脚极性 */
    if(GPFDAT & (1<<1))
        return 1;
    else
        return 0;
}
/* 外部中断1服务函数 */
void eint1_handler(void)
{
    irda_raw_event event;
    unsigned long long cur = get_system_time_us();
    /* 获取中断时间差值 */
    event.duration  =   how_long_time_us(g_last_time, cur);
    /* 获取引脚极性 */
    event.pol   = !irda_data_get();
    if(event.duration > 10000) 
    {
        /* 数据丢弃 */
    }else
    {
        /* 将获取的数据存入循环缓冲区 */
        ir_event_put(&event);
    }        
    /* 更新时间 */
    g_last_time = cur;
}

/* 获取红外原始数据 */
void irda_raw_test(void)
{
    irda_raw_event event;
    unsigned long long pre = 0, cur;  
    while(1)
    {
        /* 直到有数据时，读取当前系统时间，当两次读取间隔大于1000000us时打印回车换行 */
        if(0 == ir_event_get(&event))   
        {
            cur = get_system_time_us();

            if(how_long_time_us(pre, cur) > 1000000)
                printf("\r\n");
            pre = cur;

            printf("%s %d us |", event.pol?"high": "low", event.duration);       

        }
    }

}