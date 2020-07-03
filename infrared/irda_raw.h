#ifndef _IRDA_RAW_H
#define _IRDA_RAW_H

/* 红外原始数据结构 */
typedef struct irda_raw_event {
    int pol;        /*极性*/
    int duration;   /*脉冲宽度*/
}irda_raw_event, *p_irda_raw_event;


extern int ir_event_put(p_irda_raw_event pd);
extern int ir_event_get(p_irda_raw_event pd);
extern int ir_event_get_timeout(p_irda_raw_event pd, unsigned int timeout);
extern void irda_raw_test(void);


#endif