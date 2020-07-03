#include "s3c24xx.h"
#include "stdio.h"
#define LARGER_NAND_PAGE

#define GSTATUS1        (*(volatile unsigned int *)0x560000B0)
#define BUSY            1

#define NAND_SECTOR_SIZE    512
#define NAND_BLOCK_MASK     (NAND_SECTOR_SIZE - 1)

#define NAND_SECTOR_SIZE_LP    2048
#define NAND_BLOCK_MASK_LP     (NAND_SECTOR_SIZE_LP - 1)



//static t_nand_chip nand_chip;

/* 供外部调用的函数 */
void nand_init(void);
void nand_read(unsigned char *buf, unsigned int start_addr, unsigned int size);

/* S3C2440的NAND Flash处理函数 */
static void s3c2440_nand_reset(void);
static void s3c2440_wait_idle(void);
static void s3c2440_nand_select_chip(void);
static void s3c2440_nand_deselect_chip(void);
static void s3c2440_write_cmd(int cmd);
static void s3c2440_write_addr(unsigned int addr);
static unsigned char s3c2440_read_data(void);

int num = 12;   //测试用，当sdram初始化后，可以用位置相关代码
/* S3C2440的NAND Flash操作函数 */

/* 复位 */
static void s3c2440_nand_reset(void)
{
    s3c2440_nand_select_chip();
    s3c2440_write_cmd(0xff);  // 复位命令
    s3c2440_wait_idle();
    s3c2440_nand_deselect_chip();
}

/* 等待NAND Flash就绪 */
static void s3c2440_wait_idle(void)
{
    int i;
    while(!(NFSTAT & BUSY))
        for(i=0; i<10; i++);
}

/* 发出片选信号 */
static void s3c2440_nand_select_chip(void)
{
    int i;
   NFCONT &= ~(1<<1);
    for(i=0; i<10; i++);    
}

/* 取消片选信号 */
static void s3c2440_nand_deselect_chip(void)
{
   NFCONT |= (1<<1);
}

/* 发出命令 */
static void s3c2440_write_cmd(int cmd)
{
    NFCMD = cmd;
}

/* 发出地址 */
static void s3c2440_write_addr(unsigned int addr)
{
    int i;  
    NFADDR = addr & 0xff;
    for(i=0; i<10; i++);
    NFADDR = (addr >> 9) & 0xff;
    for(i=0; i<10; i++);
    NFADDR = (addr >> 17) & 0xff;
    for(i=0; i<10; i++);
    NFADDR = (addr >> 25) & 0xff;
    for(i=0; i<10; i++);
}

static void s3c2440_write_addr_lp(unsigned int addr)
{
	int i;
	int col, page;

	col = addr & NAND_BLOCK_MASK_LP;
	page = addr / NAND_SECTOR_SIZE_LP;
	
	NFADDR = col & 0xff;			/* Column Address A0~A7 */
	for(i=0; i<10; i++);		
	NFADDR = (col >> 8) & 0x07; 	/* Column Address A8~A11 */
	for(i=0; i<10; i++);
	NFADDR = page & 0xff;			/* Row Address A12~A19 */
	for(i=0; i<10; i++);
	NFADDR = (page >> 8) & 0xff;	/* Row Address A20~A27 */
	for(i=0; i<10; i++);
	NFADDR = (page >> 16) & 0x03;	/* Row Address A28~A29 */
	for(i=0; i<10; i++);
}

/* 读取数据 */
static unsigned char s3c2440_read_data(void)
{
   
    return NFDATA;
}



/* 初始化NAND Flash */
void nand_init(void)
{
#define TACLS   0
#define TWRPH0  3
#define TWRPH1  0
		/* 设置时序 */
        NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
        /* 使能NAND Flash控制器, 初始化ECC, 禁止片选 */
        NFCONT = (1<<4)|(1<<1)|(1<<0);
    
    
		/* 复位NAND Flash */
		s3c2440_nand_reset();
		
		num += 11;
}

#ifdef LARGER_NAND_PAGE
	#define   SECTOR_SIZE	NAND_SECTOR_SIZE_LP
#else
	#define   SECTOR_SIZE	NAND_SECTOR_SIZE  
#endif

/* 读函数 */
void nand_read(unsigned char *buf, unsigned int start_addr, unsigned int size)
{
    int i, j;
	int pre_i;
	unsigned char data;
	//j = start_addr%SECTOR_SIZE;
	if(start_addr & NAND_BLOCK_MASK_LP)	/* 必须从一页的开头开始读 */
	{
		printf("nand addr is invalid\r\n");
		return;
	}
    /* 选中芯片 */
    s3c2440_nand_select_chip();

    for(i=start_addr; i < (start_addr + size); )
	{
		/* 发出READ0命令 */
		s3c2440_write_cmd(0);

		/* Write Address */
		s3c2440_write_addr_lp(i);
	#ifdef LARGER_NAND_PAGE
		s3c2440_write_cmd(0x30);		
	#endif
		s3c2440_wait_idle();

		pre_i = i;
			
		for(j = 0; j < SECTOR_SIZE; j++, i++) {
			buf[i - start_addr] = s3c2440_read_data();		/* 读一次nand的页缓存指针往后移动一格 */	  
			  //buf++;
		}
#if 1		  
		data = s3c2440_read_data(); /* 读OOB区，如果该字节不为0xff则该块为坏块，跳过该坏块 */  
		if(data != 0xff)
		{
			i = pre_i;
		}
#endif	  
		//j = 0;
    }

    /* 取消片选信号 */
    s3c2440_nand_deselect_chip();
    
    return ;
}

/* 读函数 */
void nand_write(unsigned char *buf, unsigned int start_addr, unsigned int size)
{
    int i, j;
    if(start_addr & NAND_BLOCK_MASK_LP)	/* 必须从一页的开头开始写 */
	{
		printf("nand addr is invalid\r\n");
		return;
	}

    /* 选中芯片 */
    s3c2440_nand_select_chip();

    for(i=start_addr; i < (start_addr + size);) {
      /* 发出WRITE命令 */
      s3c2440_write_cmd(0X80);

      /* Write Address */
      s3c2440_write_addr_lp(i);
     
    for(j = 0; j < SECTOR_SIZE; j++, i++) {
          NFDATA = *buf;
          buf++;
    }

       s3c2440_write_cmd(0x10);
       s3c2440_wait_idle();

       s3c2440_write_cmd(0x70);
       if((s3c2440_read_data() & 1) != 0)
            return;
    }

    /* 取消片选信号 */
    s3c2440_nand_deselect_chip();
    
    return ;
}

