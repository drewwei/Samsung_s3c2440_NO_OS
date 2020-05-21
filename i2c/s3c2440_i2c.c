#include "../include/stdio.h"
#include "../include/s3c24xx.h"
#include "../include/int.h"
#include "../lib/string.h"
#include "../include/timer.h"
#include "i2c.h"


#if 1


volatile int flag = 1;   //注意，该处的volatile十分重要，表示告诉编译器不要到通用寄存器里取而是到内存中去读取


void AT24CXX_i2c_read(unsigned char addr, unsigned char * data, unsigned int len)
{
	volatile int i,j;	
	if((IICSTAT & (1<<5)) == 1)  //如果总线忙就直接返回
	{
		return;
	}
	//IICSTAT |= (3<<6);	    //设置为主发模式
	IICDS   = (0x50<<1);    //写设备地址	
	IICSTAT = 0xf0; 		//0.设置主发模式1.发送起始位
			
	while(flag == 1); 	//等待中断
	flag = 1;
	
	if(IICSTAT&1)     //判断是否收到ack
		DEBUG("didn't receve ack,IICSTAT is :%d\r\n",IICSTAT);
	else
		DEBUG("receve ack,IICSTAT is :%d\r\n",IICSTAT);
	
	IICDS = addr;		//写eeprom的地址
	for(j=0;j<10;j++);
	IICCON = 0xaf;   	//清挂起,回复传输
	
	while(flag == 1); 	//等待中断
	flag = 1;

	/*****可以在切换收发状态前发出stop********/
	IICSTAT = 0xd0;//停止，
	IICCON = 0xaf; //清挂起,回复传输
	mdelay(2);    //注意：经过几天的调试，发现这里要延迟2ms才能继续下一个传输
	
	//IICSTAT &=~ (3<<6);				
	//IICSTAT |= (2<<6);				//设置为主shou模式
	IICDS  = (0x50<<1)|(1<<0); 			//写设备地址
	IICSTAT = 0xb0;					//0.设置位主收模式1.发送起始位	
	
	for(j=0;j<10;j++);
	IICCON = 0xaf;					//清挂起,回复传输
	
	while(flag == 1);				//等待中断发生,说明成功发出了一个字节，从设备发出ACK,但此时从设备并没有开始发送数据，所以不能读，应该清除中断，恢复传输 
	flag = 1;
	
/**************接收****************************/
	for(i=0; i<len; i++)
	{
		if(i == (len-1)) IICCON = 0x2f;//若是最后一个数则取消ack,清挂起,恢复传输
		else 			 IICCON = 0xaf;
		while(flag == 1); 	//等待中断
		flag = 1;
		
		*(data+i) = IICDS;			//读内容
		for(j=0;j<10;j++); 			//延迟一会等IICDS被读出				
	}		
	IICSTAT = 0x90;//停止，
	IICCON = 0xaf; //清挂起,回复传输
	mdelay(2);    //注意：经过几天的调试，发现这里要延迟2ms才能继续下一个传输
}


void AT24CXX_i2c_write(unsigned char addr, unsigned char* data, unsigned int len)
{
	volatile int i,j;	
	//IICSTAT |= (3<<6);	   //设置为主发模式
	if((IICSTAT&(1<<5)) == 1)   //如果总线忙就直接返回
	{
		return;
	}
	IICDS   = 0x50<<1;   //写地址
	IICSTAT = 0xf0; //0.设置主发模式1.发送起始位,2.使能穿行输出
	

	while(flag == 1); 	//等待中断
	flag = 1;
	
	if(IICSTAT&1)     //没收到ack返回
		DEBUG("didn't receve ack,IICSTAT is :%d\r\n",IICSTAT);
	else
		DEBUG("receve ack,IICSTAT is :%d\r\n",IICSTAT);
	IICDS = addr;   //eeprom地址
	for(j=0;j<10;j++);
	IICCON = 0xaf; //清挂起,回复传输
	while(flag == 1); 	//等待中断
	flag = 1;
	
	
	for(i=0; i<len; i++) {
		IICDS =*(data+i);
		for(j=0;j<10;j++);	//延迟一会等数据被写入IICDS
		IICCON = 0xaf; //清挂起,回复传输
		while(flag == 1); 	//等待中断
		flag = 1;
		
	}		
	IICSTAT = 0xd0;//停止，
	IICCON = 0xaf; //清挂起,回复传输
	mdelay(2);		//注意：经过几天的调试，发现这里要延迟2ms才能继续下一个传输
}

#endif

struct i2c_msg *p_cur_i2c_msg;
struct i2c_controller s3c2440_i2c_controller;

void iic_init(void)
{	
	//IICCON &= ~((0xf) |(1<<6) |(1<<4));  //100khz左右的传输速度，标准速度
	//IICCON |=  (1<<7) | (1<<5) | (30<<0);//使能ACK.中断
	IICCON = (1<<7) | (0<<6) | (1<<5) | (0xf);  // 0xaf
}


/*构造I2C中断函数*/
void i2c_handler(void)
{
	//DEBUG("%s,%d,%s.\r\n",__FILE__,__LINE__,__func__);
	volatile int i;
	p_cur_i2c_msg->cnt_transferred++;
	//DEBUG("p_cur_i2c_msg->BUF:%d;p_cur_i2c_msg->addr:%d\r\n",*p_cur_i2c_msg->buf,p_cur_i2c_msg->addr);
	if(p_cur_i2c_msg->flags == TX) /*发送*/
	{		
		if(IICSTAT & 1)     //判断是否收到ack
		{
			printf("didn't receve ack,IICSTAT is :%d\r\n",IICSTAT);
			IICSTAT = 0xd0; //停止，
			IICCON  = 0xaf;  //清挂起,回复传输
			mdelay(2);		
			return;
		}
		else{
			//printf("receve ack,IICSTAT is :%d\r\n",IICSTAT);
		}
				
		if(p_cur_i2c_msg->cnt_transferred == p_cur_i2c_msg->len)
		{
			IICSTAT = 0xd0; //停止，
			IICCON  = 0xaf;  //清挂起,回复传输
			
			mdelay(2);	
		}
		else{	
			IICDS = p_cur_i2c_msg->buf[p_cur_i2c_msg->cnt_transferred]; /*将当前已经传输的个数传入*/
			//DEBUG("%s,%d,%s.IICDS:%d\r\n",__FILE__,__LINE__,__func__,IICDS);
			for(i=0;i<10;i++); 	//等待数据被写入IICDS
			IICCON = 0xaf; 		//清挂起,恢复传输		
		}
		
	}
	else
	{	   /* 接收模式 msg[0].flags = RX;
			* 接收必须先发出从设备地址，然后才能真正的进入接收状态
			* do_master_rx中已经发出从设备地址，进入中断后进入该分支
			* 如果是第一次进入中断，则应该判断从设备是否发出的ACK
			*/	
			if(p_cur_i2c_msg->cnt_transferred == 0) /*发送设备地址后第一次发生中断,这里要清中断以便恢复传输接收从设备的数据*/
			{
				if(IICSTAT & 1)     //判断是否收到ack
				{
					printf("didn't receve ack,IICSTAT is :%d\r\n",IICSTAT);
					IICSTAT = 0x90; //停止，
					IICCON  = 0xaf;  //清挂起,回复传输
	
					mdelay(3);		
					return;
				}
				else {
					//printf("receve ack,IICSTAT is :%d\r\n",IICSTAT);
				}
									
				if(p_cur_i2c_msg->cnt_transferred == (p_cur_i2c_msg->len - 1))  /*如果下一个中断接收最后一个数据则禁止ACK*/
					IICCON = 0x2f;//若是最后一个数则取消ack,清挂起,恢复传输
				else 			 
					IICCON = 0xaf;
			}
			else{					
				p_cur_i2c_msg->buf[p_cur_i2c_msg->cnt_transferred - 1] = IICDS;
				//DEBUG("p_cur_i2c_msg->cnt_transferred:%d.IICDS:%d\r\n",p_cur_i2c_msg->cnt_transferred, IICDS);
				/*判断是不是最后一个数*/				
				if(p_cur_i2c_msg->cnt_transferred == p_cur_i2c_msg->len)
				{
					IICSTAT = 0x90;//停止，
					IICCON  = 0xaf; //清挂起,回复传输
					mdelay(3);		//注意：经过几天的调试，发现这里要延迟2ms才能继续下一个传输
				}
				else{
					/*判断是不是倒数第二个数*/
					if(p_cur_i2c_msg->cnt_transferred == (p_cur_i2c_msg->len - 1))  /*如果是倒数第二个数则禁止ACK*/
						IICCON = 0x2f;//若是最后一个数则取消ack,清挂起,恢复传输
					else
						IICCON = 0xaf;
				}
								
			}	
		
		}
}

/*5.构造do_master_tx函数*/
static void do_master_rx(struct i2c_msg *msg)
{
	DEBUG("%s,%d,%s.\r\n",__FILE__,__LINE__,__func__);
	p_cur_i2c_msg = msg;
	if((IICSTAT&(1<<5)) == 1)   //如果总线忙就直接返回
	{
		uart_send("i2c bus busy.\r\n");
		return;
	}
	msg->cnt_transferred = -1;
	/*1.设置为主发模式*/	

	/*2.Write 0xB0 (M/RStart) to IICSTAT*/
	IICSTAT = 0xb0;
	/*3.发送设备地址The data of the IICDS (slaveaddress) is transmitted*/
	IICDS   = (msg->addr<<1) | 1;

	
	while(msg->cnt_transferred != msg->len);
}


/*4.构造do_master_tx函数*/
static void do_master_tx(struct i2c_msg *msg)
{	
	p_cur_i2c_msg = msg;
	if((IICSTAT&(1<<5)) == 1)   //如果总线忙就直接返回
	{
		uart_send("i2c bus busy.\r\n");
		return;
	}
	msg->cnt_transferred = -1;

	
	/*1.设置为主发模式*/
	IICSTAT |= (3<<6);	
	/*2.Write 0xF0 (M/TStart) to IICSTAT*/
	IICSTAT = 0xf0;
	/*3.发送设备地址*/
	IICDS	= (msg->addr)<<1;
	
	
	
	DEBUG("%s,%d,%s.\r\n",__FILE__,__LINE__,__func__);
	
	while(msg->cnt_transferred != msg->len);
}

/*3.I2C传输函数*/
static void s3c2440_master_xfer(struct i2c_msg *msgs,unsigned int cnt)
{
	DEBUG("%s,%d,%s.\r\n",__FILE__,__LINE__,__func__);
	int i;
	for(i = 0; i < cnt; i++)
	{
		if(msgs->flags == TX)
			do_master_tx(&msgs[i]);
		else
			do_master_rx(&msgs[i]);
	}
}

/*2.构造s3c2440主机控制器*/

void init_s3c2440_i2c_controller(struct i2c_controller *s3c2440_i2c_controller)
{
	s3c2440_i2c_controller->name 		= "s3c440i2c";
	s3c2440_i2c_controller->init 		= iic_init;
	s3c2440_i2c_controller->master_xfer	= s3c2440_master_xfer;
	
}
























