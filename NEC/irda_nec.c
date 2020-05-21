#include "irda_raw.h"
#include "../include/stdio.h"
#include "../include/timer.h"
/*持续时间*/
#define DURATION_BASE  563
#define DURATION_DELTA (DURATION_BASE/2)

/*低脉冲头信息*/
#define DURATION_HEAD_LOW    (16*DURATION_BASE)
/*高脉冲头信息*/
#define DURATION_HEAD_HIGH   (8*DURATION_BASE)
/*高脉冲重复码*/
#define DURATION_REPEAT_HIGH (4*DURATION_BASE)
/*低脉冲传输数据*/
#define DURATION_DATA_LOW    (1*DURATION_BASE)
/*数据1高脉冲*/
#define DURATION_DATA1_HIGH  (3*DURATION_BASE)
/*数据0高脉冲*/
#define DURATION_DATA0_HIGH  (1*DURATION_BASE)
/*低脉冲结束码*/
#define DURATION_END_LOW     (1*DURATION_BASE)


#define MAX		3
extern volatile int offset;
extern const unsigned char *p[];
extern void show_pic_16(const unsigned char * database);
static int duration_in_margin(int duration, int us)
{
	if ((duration > (us - DURATION_DELTA)) && (duration < us + DURATION_DELTA))
		return 1;
	else
		return 0;
}

/*
 * 返回值: 0-得到数据, 1-得到重复码, -1 : 失败
 */
int irda_nec_read(int *address, int *data)
{
	irda_raw_event event;	
	int i;
	unsigned int val = 0;
	unsigned char byte[4];

while (1)
	{
		if (0 == ir_event_get(&event))
		{
			/* 解析数据 */
			/* 1. 判断是否为9MS的低脉冲 */
			if (event.pol == 0 && \
				duration_in_margin(event.duration, DURATION_HEAD_LOW))
			{
				/* 进行后续判断 */
				/* 2. 读下一个高脉冲数据 */
				if (0 == ir_event_get_timeout(&event, 10000))
				{
					/* 3. 判断它是否4.5ms的高脉冲
					 *    或者 2.25ms的高脉冲
					 */
					if (event.pol == 1 && \
						duration_in_margin(event.duration, DURATION_HEAD_HIGH))
					{
						/* 4.5ms的高脉冲 */
						/* 4. 重复解析32位数据 */
						for (i = 0; i < 32; i++)
						{
							/* 5. 读0.56ms的低脉冲 */
							if (0 == ir_event_get_timeout(&event, 10000))
							{
								if (event.pol == 0 && \
									duration_in_margin(event.duration, DURATION_DATA_LOW))
								{
									/* 6. 读下一个数据, 判断它是 0.56ms/1.68ms的高脉冲 */
									if (0 == ir_event_get_timeout(&event, 10000))
									{
										if (event.pol == 1 && \
											duration_in_margin(event.duration, DURATION_DATA1_HIGH))
										{
											/* 得到了bit 1 */
											val |= (1<<i);
										}
										else if (event.pol == 1 && \
												duration_in_margin(event.duration, DURATION_DATA0_HIGH))
										{
											/* 得到了bit 0 */
										}
										else
										{
											printf("%s %d\n\r", __FUNCTION__, __LINE__);
											return -1;
										}
									}
									else
									{
										printf("%s %d\n\r", __FUNCTION__, __LINE__);
										return -1;
									}
								}
								else
								{
									printf("%s %d\n\r", __FUNCTION__, __LINE__);
									return -1;
								}
							}
							else
							{
								printf("%s %d\n\r", __FUNCTION__, __LINE__);
								return -1;
							}
						}

						/* 7. 得到了32位数据, 判断数据是否正确 */
						byte[0] = val & 0xff;
						byte[1] = (val>>8) & 0xff;
						byte[2] = (val>>16) & 0xff;
						byte[3] = (val>>24) & 0xff;

						//printf("get data: %x %x %x %x\n\r", byte[0], byte[1], byte[2], byte[3]);
						byte[1] = ~byte[1];
						byte[3] = ~byte[3];
						
						if (byte[0] != byte[1])
						{
							/* 有些遥控器不完全遵守NEC规范 */
							//printf("%s %d\n\r", __FUNCTION__, __LINE__);
							//return -1;
						}
						if (byte[2] != byte[3])
						{
							printf("%s %d\n\r", __FUNCTION__, __LINE__);
							return -1;
						}
						*address = byte[0];
						*data    = byte[2];
						return 0;
						
					}
					else if (event.pol == 1 && \
						duration_in_margin(event.duration, DURATION_REPEAT_HIGH))
					{
						/* 2.25ms的高脉冲 */
						return 1;  /* 重复码 */
					}
					else
					{
						printf("%s %d\n\r", __FUNCTION__, __LINE__);
						return -1;  /* 错误 */
					}
				}
			}
			else
			{
				//printf("%s %d\n\r", __FUNCTION__, __LINE__);
				return -1; /* 有效数据未开始 */
			}
		}
	}
}

void irda_nec_test(void)
{
	int address;
	int data;
	int ret;
	
	while (1)
	{
		ret = irda_nec_read(&address, &data);
		if (ret == 0)
		{	/*输出地址和数据*/
			printf("irda_nec_read: address = 0x%x, data = 0x%x\n\r", address, data);
		}
		else if (ret == 1)
		{
			printf("irda_nec_read: repeat code\n\r");
		}

		if(data == 0x43)
		{
			if(offset > 0)
			{
				show_pic_16(p[--offset]);
			}
			else if(offset == 0)
			{
				offset = MAX-1;
				show_pic_16(p[offset]);
			}
			
			data = 0;
		}
		else if(data == 0x40)
		{	
			if(offset< (MAX-1))
			{
				show_pic_16(p[++offset]);
			}
			else if(offset == (MAX-1))
			{
				offset = 0;
				show_pic_16(p[offset]);
			}
			data = 0;
		}
		

	}


	
}
