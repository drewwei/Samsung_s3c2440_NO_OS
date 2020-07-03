#include "../../include/stdio.h"
#include "iis.h"

#define DMA_MAXLEN		0x1ffffe
#define DMA0_BASE_ADDR  0x4B000000
#define DMA1_BASE_ADDR  0x4B000040
#define DMA2_BASE_ADDR  0x4B000080
#define DMA3_BASE_ADDR  0x4B0000C0

struct s3c_dma_regs {
	unsigned long disrc;
	unsigned long disrcc;
	unsigned long didst;
	unsigned long didstc;
	unsigned long dcon;
	unsigned long dstat;
	unsigned long dcsrc;
	unsigned long dcdst;
	unsigned long dmasktrig;
};

struct s3c_dma_regs *dma_regs;

static int dma_src_addr;
static int dma_src_origin;
static int dma_len_origin;
static int tollen;
void dma_init(unsigned int src, unsigned int len)
{
	static int i = 0;
	dma_src_addr = src;
	if(i == 0)	/* 第一次进入该函数时执行 */
	{
		tollen = len;	/* 记录总数据长度 */
		dma_len_origin = len;	/* 记录最初的DMA总数据长度 */
		dma_src_origin = src;	/* 记录最初的DMA源地址 */	
		i++;
	} 
	if(len > DMA_MAXLEN)	/* 如果传输总长度小于DMA_MAXLEN 则本次传输的字节数为tollen */
	{
		len = DMA_MAXLEN;  /* 否则则本次传输的字节数为 一次最大DMA传输字节 */
	}
	

	dma_regs = (struct s3c_dma_regs *)DMA2_BASE_ADDR;
	/* 把源,目的,长度告诉DMA */
	dma_regs->disrc      = src;        /* 源的物理地址 */
	dma_regs->disrcc     = (0<<1) | (0<<0); /* 源位于AHB总线, 源地址递增 */
	dma_regs->didst      = 0x55000010;        /* 目的的物理地址 */
	dma_regs->didstc     = (0<<2) | (1<<1) | (1<<0); /* 目的位于APB总线, 目的地址不变 */
	dma_regs->dcon       = (1<<31)|(0<<30)|(1<<29)|(0<<28)|(0<<27)|(0<<24)|(1<<23)|(1<<22)|(1<<20)|((len/2) & (0xfffff));  /* 使能中断,单个传输,硬件触发 */
	
}
void dma_start(void)
{
	/* 启动DMA */
	dma_regs->dmasktrig  = (1<<1);
}

void dma_stop(void)
{
	/* 启动DMA */
	dma_regs->dmasktrig  &= ~(1<<1);
}

#define PLAY_LOOP

static void dma_repeat(void)
{
	dma_src_addr += DMA_MAXLEN;		/* DMA源地址增加DMA_MAXLEN */
	tollen 		 -= DMA_MAXLEN;		/* DMA总传输长度减DMA_MAXLEN */

	if(tollen > 0)
	{
		if(tollen < DMA_MAXLEN)		/* 如果剩余字节数小于最大DMA传输长度，则DMA传输字节为tollen */
		{
			dma_init(dma_src_addr, tollen);	
		}
		else{	
			dma_init(dma_src_addr, DMA_MAXLEN);		/* 否则 以最大DMA数据长度传输 */
		}	
		dma_start();		
	}else
	{
#ifdef PLAY_LOOP
		tollen = dma_len_origin;		
		dma_init(dma_src_origin, dma_len_origin);	/* 重复播放 */
		dma_start();
#else
		dma_stop();		/* 单次播放。当传输完整个tollen字节后停止播放 ，DMA计数器减到0自动停止DMA */
#endif 
	}
}

/* DMA2中断服务函数 */
void DMA2IntHandle(void)
{
	printf("dma done\r\n");
	dma_repeat();
}























