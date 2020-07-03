
#ifndef __SPI_FLASH__
#define __SPI_FLASH__

#include "ctl_spi.h"
extern void ReadID(unsigned char *PMID, unsigned char *PDID);

extern void SpiFlashReadData(unsigned int addr, unsigned char *data, int size);

extern void SpiFlashWriteData(unsigned int addr, const unsigned char *data, int size);
extern void ChipErase();

extern void InitSpiFlash();



#endif