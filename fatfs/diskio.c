/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "../spi/include/spi_flash.h"
#include "stdio.h"
/* Definitions of physical drive number for each drive */
#define DEV_FLASH	0	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_RAM		1	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define SECTOR_SIZE			4096

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv) {
	case DEV_RAM :
		return RES_OK;

	case DEV_FLASH :
		

		// translate the reslut code here

		return RES_OK;

	case DEV_USB :

		return RES_OK;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{

	switch (pdrv) {
	case DEV_RAM :

		return RES_OK;
	case DEV_FLASH :
		// translate the reslut code here
		/* 已经初始化 */
		printf("SPIFlashInit\r\n");
		return RES_OK;

	case DEV_USB :
		
		return RES_OK;
	}
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/



DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	//DRESULT res;
	unsigned int addr = sector*SECTOR_SIZE;
	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return RES_PARERR;

	case DEV_FLASH :
		// translate the arguments here
		
		SpiFlashReadData(addr, buff, count*SECTOR_SIZE);

		// translate the reslut code here

		return RES_OK;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return RES_PARERR;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	//int result;
	unsigned int addr = sector*SECTOR_SIZE;
	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return RES_PARERR;

	case DEV_FLASH :
		// translate the arguments here
	
	
			SpiFlashWriteData(addr, buff, count*SECTOR_SIZE);			

		// translate the reslut code here

		return RES_OK;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return RES_PARERR;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
	//int result;

	switch (pdrv) {
	case DEV_RAM :

			// Process of the command for the RAM drive

			return RES_OK ;

	case DEV_FLASH :
			switch (cmd)
			{
			case GET_SECTOR_COUNT:
				*(LBA_t *)buff = 512;
				res = RES_OK;
				break;
			case GET_SECTOR_SIZE:
				*(WORD *)buff = SECTOR_SIZE;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE:
				*(DWORD *)buff = 1;
				res = RES_OK;
				break;
			case CTRL_SYNC:
				res = RES_OK;
				break;
			default:
				printf("disk_ioctl\r\n");
				res = RES_PARERR;
				break;
			}	

		// Process of the command for the MMC/SD card
			return res;

	case DEV_USB :

		// Process of the command the USB drive

		return RES_OK ;
	}

	return res;
}

DWORD get_fattime (void)
{
	return 0;
}
