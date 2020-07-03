#include "irda_raw.h"
#include "../include/timer.h"
//#define NEXT_PALCE(i)   (i+1)%1024
#define NEXT_PALCE(i)   ((i+1)&0x3ff)

static irda_raw_event g_event[1024];
static volatile int g_r = 0;
static volatile int g_w = 0;

static int is_irda_buf_empty(void)
{
    return (g_r == g_w);
}
static int is_irda_buf_full(void)
{
    return (NEXT_PALCE(g_w) == g_r);
}

int ir_event_put(p_irda_raw_event pd)
{
    if(is_irda_buf_full()) 
        return -1;
    g_event[g_w] = *pd;
    g_w = NEXT_PALCE(g_w);
    return 0;
}
int ir_event_get(p_irda_raw_event pd)
{
    if(is_irda_buf_empty())
        return -1;
    *pd = g_event[g_r];
    g_r = NEXT_PALCE(g_r);
    return 0;
}
int ir_event_get_timeout(p_irda_raw_event pd, unsigned int timeout)
{
	//int i = timeout;
/*
    while(is_irda_buf_empty())
    {
        printf("irda_buf_empty\r\n");
        if(i--)  
            continue;
        return -1;
	}
    printf("irda buf not empty\r\n");
    *pd = g_event[g_r];
	g_r = NEXT_PALCE(g_r);
   	return 0; 	
*/
	while (timeout--)
	{
		if (!is_irda_buf_empty())
		{
			*pd = g_event[g_r];
			g_r = NEXT_PALCE(g_r);
			
			return 0;
		}
		udelay(10);
	}
	return -1;     
}