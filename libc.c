#define NULL ((void *)0)
char itocBuff[128] = {0};
char *itoc(const int value)//整形转字符串
{
	char debuff[128];
	int a = value, index = 0, deIndex = 0;
	while ((a / 10) != 0)
	{
		debuff[deIndex++] = a % 10 + 0x30;
		a /= 10;
	}
    debuff[deIndex] = a % 10 + 0x30;
    while (deIndex >= 0)
    {
        itocBuff[index++] = debuff[deIndex--];
    }
    itocBuff[index] = '\0';
    return itocBuff;
}

void memset(void *dest, int set, unsigned int len)
{
	char * pdest = (char *)dest;
	while(len--)
	{
		*pdest = set;
		pdest++;
	}	
}
unsigned int strlen(char * str)
{
	if(str == NULL )
		return -1;
	int i = 0;
	while(str[i++]);	
	return i-1;
}


void * memcpy(void *dest, void *src, unsigned int len)
{
	if(dest == NULL || src == NULL || len == 0)
		return NULL;
	char * pdest = (char *)dest;
	char *psrc	 = (char *)src;
	if(((int)pdest > (int)src) && (((int)psrc+len) > (int)pdest))
	{
		pdest += len -1;
		psrc  += len -1;
		while(len--)
			*pdest-- = *psrc--;	
	}		
	else
	{
		while(len--)
		*pdest++ = *psrc++;
	}
	
	return dest;
}

void * memcpy_int(void *dest, void *src, unsigned int len)
{
	if(dest == NULL || src == NULL || len == 0)
		return NULL;
	int * pdest = (int *)dest;
	int *psrc	 = (int *)src;
	char *temp1;
	char *temp2;
	int c1 = len/4;
	int c2 = len%4;
	if(((int)pdest > (int)src) && (((int)psrc+len) > (int)pdest))  //强制类型转换只临时的，不改变数据类型
	{
		temp1 = (char *)pdest;
		temp2 = (char *)psrc;  
		
		temp1 += len -1;
		temp2 += len -1;
		while(c2--)
			*temp1-- = *temp2--;	
		
		pdest = (int *)temp1;
		psrc = (int *)temp2; 
		while(c1--)
		{
			*pdest-- = *psrc--;	
		}
	}		
	else
	{
		while(c1--)
		{
			*pdest++ = *psrc++;
		}
		temp1 = (char *)pdest;
		temp2 = (char *)psrc;  
		while(c2--)
		{
			*temp1-- = *temp2--;	
		}
	}
	
	return dest;
}

int*  strcpy(char *dest, char *src)
{
	if(dest == NULL || src == NULL)
		return 0;
	while(*dest++ = *src++ != '\0');
		
}

int strcmp(const char *dest, const char *src)
{
	if(dest == NULL || src == NULL)
		return 1;
	/*
	while(*dest++ == *src++ )
	{
		if(*dest == '\0')
			return 0;
	}
	*/
	int ret;
	while(!(ret = (*dest - *src)) && *src)
	{
		dest++;
		src++;
	}
	return ret;
}


























