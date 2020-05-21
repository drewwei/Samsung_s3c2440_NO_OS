#include "stdio.h"
#include "s3c24xx.h"
#include "int.h"
#include "lib/string.h"
#include "timer.h"
void init_gpio(void)
{
	GPFCON &= ~((3 << (4*2)) | (3 << (5*2)) | (3 << (6*2)));	
	GPFCON |= (OUTPUT << (4*2)) | (OUTPUT << (5*2)) | (OUTPUT << (6*2)); /* GPF4,GPF5,GPF6配置为输出*/
#if 0
	GPFCON &= ~((3 << (0*2)) | (3 << (2*2)));	
	GPFCON |= (EINT << (0*2)) | (EINT << (2*2));						 /* GPF0,GPF2配置为中断模式 */
	
	GPGCON &= ~((3 << 6) | (3 << 22));									 
	GPGCON |= (EINT << 6) | (EINT << 22);								 /* GPG3,GPG11 配置为中断模式*/
		
#endif	


	/*************uart******************/
	GPHCON &= ~((3 << 4) | (3 << 6));   								 /*	GPH2,GPH3 配置位uart*/
	GPHCON |=  ((2 << 4) | (2 << 6));
	GPHUP  &= ~((1 << 2) | (1 << 3));										/*  使能内部上拉*/
	/*************IIC************************/
	GPECON &= ~((3 << 28)|(3 << 30));										/* GPE14,GPE15配置成iic*/
	GPECON |=  ((2 << 28)|(2 << 30));
	GPEUP  |=  0xc000;       // 禁止内部上拉

}




















