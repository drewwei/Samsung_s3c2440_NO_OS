#include "dma.h"
#include "s3c24xx.h"
#define LCD_FB_BASE		0X33C00000

 P_DMA_Struct p_dma0_t = (P_DMA_Struct)DMA0_base;
void DMA0_Init(void)
{
   

    p_dma0_t->DISRC  = 0x50000024;          //源地址为uart1的发送寄存器
    p_dma0_t->DISRCC = (1<<1)|(1<<0);       //源地址在APB总线，地址Fixed模式
    p_dma0_t->DIDST  = LCD_FB_BASE;         //目的地址时LCD显存首地址
    p_dma0_t->DIDSTC = (0<<1) | (0<<0);     //目的地址在AHB总线，地址增长模式
    p_dma0_t->DCON  &= ~((7<<24) | (3<<20));//传输单位Byte    
    p_dma0_t->DCON   = (1<<29) | (1<<24) | (1<<23) | (1<<22);             //uart0触发DMA传输,不重载,使能DMA中断,不reload DMA只能触发一次

     p_dma0_t->DCON |= 480*272*2;
}
void Start_DMA(void)
{

    while((p_dma0_t->DSTAT & (0x3<<20)) != 0); 
    p_dma0_t->DMASKTRIG = (1<<1); //硬件触发
    p_dma0_t->DMASKTRIG &= ~(1<<2);
}
void Stop_DMA(void)
{
    /*停止DMA*/
    p_dma0_t->DMASKTRIG |= (1<<2);
}

void DMA0_handler(void)
{
    /*停止DMA使DMA重置复位*/
     Stop_DMA();
     nand_write(LCD_FB_BASE, 0x100000, 272*480*2);
   /*由于不reload,DMA被自动关闭，不再触发，所以重新启动*/
    // Start_DMA();
}