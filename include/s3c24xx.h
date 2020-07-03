/* WOTCH DOG register */
#define     WTCON           (*(volatile unsigned long *)0x53000000)

/* SDRAM regisers */
#define     MEM_CTL_BASE    0x48000000
#define     SDRAM_BASE      0x30000000

/* NAND Flash registers */
#define NFCONF              (*(volatile unsigned int  *)0x4E000000)
#define NFCONT              (*(volatile unsigned char *)0x4E000004)
#define NFCMD               (*(volatile unsigned char *)0x4E000008)
#define NFADDR              (*(volatile unsigned char *)0x4E00000C)
#define NFDATA              (*(volatile unsigned char *)0x4E000010)
#define NFSTAT              (*(volatile unsigned char *)0x4E000020)

/*i2c registers*/
#define IICCON              (*(volatile unsigned char *)0x54000000)
#define IICSTAT             (*(volatile unsigned char *)0x54000004)
#define IICADD              (*(volatile unsigned char *)0x54000008)
#define IICDS               (*(volatile unsigned char *)0x5400000C)
/* SPI registers */
#define	SPCON0		(*(volatile unsigned long *)0x59000000)
#define	SPSTA0		(*(volatile unsigned long *)0x59000004)
#define	SPPIN0		(*(volatile unsigned long *)0x59000008)
#define	SPPRE0		(*(volatile unsigned long *)0x5900000C)
#define	SPTDAT0	    (*(volatile unsigned char *)0x59000010)
#define	SPRDAT0	    (*(volatile unsigned char *)0x59000014)

#define SPCON1   	(*(volatile unsigned *)0x59000020) // spi1 control
#define SPSTA1  	(*(volatile unsigned *)0x59000024) // spi1 status
#define SPPRE1 		(*(volatile unsigned *)0x5900002C) //Baud rate = PCLK / 2 / (Prescaler value + 1) 
#define SPTDAT1   	(*(volatile unsigned *)0x59000030) // 
#define SPRDAT1    	(*(volatile unsigned *)0x59000034) // 


/*GPIO registers*/
#define GPBCON              (*(volatile unsigned long *)0x56000010)
#define GPBDAT              (*(volatile unsigned long *)0x56000014)
#define GPECON              (*(volatile unsigned long *)0x56000040)
#define GPEUP              (*(volatile unsigned long *)0x56000048)

#define GPCCON              (*(volatile unsigned long *)0x56000020)
#define GPCDAT              (*(volatile unsigned long *)0x56000024)
#define GPCUP               (*(volatile unsigned long *)0x56000028)

#define GPDCON              (*(volatile unsigned long *)0x56000030)
#define GPDDAT              (*(volatile unsigned long *)0x56000034)
#define GPDUP               (*(volatile unsigned long *)0x56000038)

#define GPECON    			(*(volatile unsigned long *)0x56000040)
#define GPEDAT    		    (*(volatile unsigned long *)0x56000044)
#define GPEUP     			(*(volatile unsigned long *)0x56000048)

#define GPFCON              (*(volatile unsigned long *)0x56000050)
#define GPFDAT              (*(volatile unsigned char *)0x56000054)
#define GPFUP               (*(volatile unsigned char *)0x56000058)

#define GPGCON              (*(volatile unsigned long *)0x56000060)
#define GPGDAT              (*(volatile unsigned long *)0x56000064)
#define GPGUP               (*(volatile unsigned long *)0x56000068)

#define GPHCON              (*(volatile unsigned long *)0x56000070)
#define GPHDAT              (*(volatile unsigned long *)0x56000074)
#define GPHUP               (*(volatile unsigned long *)0x56000078)
// LCD CONTROLLER
#define LCDCON1     (*(volatile unsigned long *)0x4d000000) //LCD control 1
#define LCDCON2     (*(volatile unsigned long *)0x4d000004) //LCD control 2
#define LCDCON3     (*(volatile unsigned long *)0x4d000008) //LCD control 3
#define LCDCON4     (*(volatile unsigned long *)0x4d00000c) //LCD control 4
#define LCDCON5     (*(volatile unsigned long *)0x4d000010) //LCD control 5
#define LCDSADDR1   (*(volatile unsigned long *)0x4d000014) //STN/TFT Frame buffer start address 1
#define LCDSADDR2   (*(volatile unsigned long *)0x4d000018) //STN/TFT Frame buffer start address 2
#define LCDSADDR3   (*(volatile unsigned long *)0x4d00001c) //STN/TFT Virtual screen address set
#define REDLUT      (*(volatile unsigned long *)0x4d000020) //STN Red lookup table
#define GREENLUT    (*(volatile unsigned long *)0x4d000024) //STN Green lookup table 
#define BLUELUT     (*(volatile unsigned long *)0x4d000028) //STN Blue lookup table
#define DITHMODE    (*(volatile unsigned long *)0x4d00004c) //STN Dithering mode
#define TPAL        (*(volatile unsigned long *)0x4d000050) //TFT Temporary palette
#define LCDINTPND   (*(volatile unsigned long *)0x4d000054) //LCD Interrupt pending
#define LCDSRCPND   (*(volatile unsigned long *)0x4d000058) //LCD Interrupt source
#define LCDINTMSK   (*(volatile unsigned long *)0x4d00005c) //LCD Interrupt mask
#define LPCSEL      (*(volatile unsigned long *)0x4d000060) //LPC3600 Control
#define PALETTE     0x4d000400                              //Palette start address
/* CLOCK register*/
#define MPLLCON             0x4C000004
#define CLKDIVN             0x4C000014

#define CLKCON               (*(volatile unsigned long *)0x4C00000C)


/*UART registers*/
#define ULCON0              (*(volatile unsigned long *)0x50000000)
#define UCON0               (*(volatile unsigned long *)0x50000004)
#define UFCON0              (*(volatile unsigned long *)0x50000008)
#define UMCON0              (*(volatile unsigned long *)0x5000000c)
#define UTRSTAT0            (*(volatile unsigned long *)0x50000010)
#define UTXH0               (*(volatile unsigned char *)0x50000020)
#define URXH0               (*(volatile unsigned char *)0x50000024)
#define UBRDIV0             (*(volatile unsigned long *)0x50000028)
#define UFSTAT0             (*(volatile unsigned long *)0x50000018)

/*interrupt registes*/
#define SRCPND              (*(volatile unsigned long *)0x4A000000)
#define INTMOD              (*(volatile unsigned long *)0x4A000004)
#define INTMSK              (*(volatile unsigned long *)0x4A000008)
#define PRIORITY            (*(volatile unsigned long *)0x4A00000c)
#define INTPND              (*(volatile unsigned long *)0x4A000010)
#define INTOFFSET           (*(volatile unsigned long *)0x4A000014)
#define SUBSRCPND           (*(volatile unsigned long *)0x4A000018)
#define INTSUBMSK           (*(volatile unsigned long *)0x4A00001c)

/*external interrupt registers*/
#define EINTMASK            (*(volatile unsigned long *)0x560000a4)
#define EINTPEND            (*(volatile unsigned long *)0x560000a8)
#define EXTINT0             (*(volatile unsigned long *)0x56000088)
#define EXTINT1             (*(volatile unsigned long *)0x5600008c)
#define EXTINT2             (*(volatile unsigned long *)0x56000090)
#define INTOFFSET           (*(volatile unsigned long *)0x4A000014)
/* timer4 register*/
#define TCFG0           	(*(volatile unsigned long *)0x51000000)
#define TCFG1          		(*(volatile unsigned long *)0x51000004)
#define TCON           		(*(volatile unsigned long *)0x51000008)
#define TCNTB4          	(*(volatile unsigned long *)0x5100003C)
#define TCNTO4           	(*(volatile unsigned long *)0x51000040)
#define TCNTB0          	(*(volatile unsigned long *)0x5100000C)
#define TCNTO0           	(*(volatile unsigned long *)0x51000014)
/* DMA register */
#define DISRC0           	(*(volatile unsigned long *)0x4B000000)
#define DISRCC0           	(*(volatile unsigned long *)0x4B000004)
#define DIDST0           	(*(volatile unsigned long *)0x4B000008)
#define DIDSTC0           	(*(volatile unsigned long *)0x4B00000C)
#define DCON0           	(*(volatile unsigned long *)0x4B000010)
#define DSTAT0           	(*(volatile unsigned long *)0x4B000014)
#define DCSRC0           	(*(volatile unsigned long *)0x4B000018)
#define DCDST0           	(*(volatile unsigned long *)0x4B00001C)
#define DMASKTRIG0          (*(volatile unsigned long *)0x4B000020)
#define DMA0_base           (volatile unsigned long *)0x4B000000


#define INPUT					0
#define OUTPUT					1
#define EINT					2
#define Low_level  				0
#define High_level  			1
#define Falling_edge_triggered  2
#define Falling_edge_triggered  2
#define Both_edge_triggered     6





