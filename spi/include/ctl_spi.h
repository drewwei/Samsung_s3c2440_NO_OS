#ifndef __GPIO_SPI__
#define __GPIO_SPI__
extern void GPIO_SPI_Init();
extern void SPI_SendByte(unsigned char c);
extern unsigned char SPI_RevByte();
extern void ConfigSPI(int mode);
#endif