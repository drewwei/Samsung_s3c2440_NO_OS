#ifndef		_SOUND_DMA_H
#define		_SOUND_DMA_H

void dma_init(unsigned int src, unsigned int len);
void dma_start(void);
void dma_stop(void);
void DMA2IntHandle(void);



#endif