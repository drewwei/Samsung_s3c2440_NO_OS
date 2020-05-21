#include "vsprintf.h"
#include "string.h"
#include "printf.h"
#include "s3c24xx.h"

extern void putc(unsigned char c);
extern unsigned char getc(void);

#define	OUTBUFSIZE	1024
#define	INBUFSIZE	1024


static unsigned char g_pcOutBuf[OUTBUFSIZE];
static unsigned char g_pcInBuf[INBUFSIZE];


int printf(const char *fmt, ...)
{
	int i;
	int len;
	va_list args;

	va_start(args, fmt);
	len = vsprintf(g_pcOutBuf,fmt,args);
	va_end(args);
	for (i = 0; i < strlen(g_pcOutBuf); i++)
	{
		putc(g_pcOutBuf[i]);
	}
	return len;
}



int scanf(const char * fmt, ...)
{
	int i = 0;
	unsigned char c;
	va_list args;
	
	while(1)
	{
		c = getc();
		if((c == 0x0d) || (c == 0x0a))
		{
			g_pcInBuf[i] = '\0';
			if(c == 0x0a)
				break;
		}
		else
		{
			g_pcInBuf[i++] = c;
		}
	}
	
	va_start(args,fmt);
	i = vsscanf(g_pcInBuf,fmt,args);
	va_end(args);

	return i;
}
int atoi(char *str)//字符串转数字 
{
	char flag = '+';//指示结果是否带符号 
	long res = 0;
	
	if(*str=='-')//字符串带负号 
	{
		++str;//指向下一个字符 
		flag = '-';//将标志设为负号 
	} 
	//逐个字符转换，并累加到结果res 
	while(*str>=48 && *str<57)//如果是数字才进行转换，数字0~9的ASCII码：48~57 
	{
		res = 10*res+  *str++-48;//字符'0'的ASCII码为48,48-48=0刚好转化为数字0 
	} 
 
    if(flag == '-')//处理是负数的情况
	{
		res = -res;
	}
 
	return (int)res;
}