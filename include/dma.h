#ifndef      __DMA__
#define      __DMA__  

typedef struct DMA_Struct
{
    unsigned int DISRC;    
    unsigned int DISRCC;   
    unsigned int DIDST;    
    unsigned int DIDSTC;   
    unsigned int DCON ;     
    unsigned int DSTAT;    
    unsigned int DCSRC;    
    unsigned int DCDST;    
    unsigned int DMASKTRIG;
}*P_DMA_Struct, DMA_Struct;


void DMA0_Init(void);
void Start_DMA(void);

#endif // __DMA__