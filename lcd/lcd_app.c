//#include "../include/picture.h"
#include "../lib/string.h"
//#include "../include/pic.h"
//#include "../include/shaohui.h"
//#include "../include/zhang.h"
//#include "picture/zhang1.h"
//#include "picture/shaohui1.h"
//#include "picture/pic.h"
#define LCD_FB_BASE		0X33C00000

//const unsigned char *p[] = {gImage_pic};  /*picture 的首地址数组, gImage_zhang1, gImage_shaohui, gImage_zhang*/

void show_pic_16(const unsigned char * database)
{
/*
	unsigned int *p2 = (unsigned int *)LCD_FB_BASE;
	int x,y,z=0;
	

	if(database == gImage_zhang)
	{
		unsigned int *temp = (unsigned int *)gImage_zhang;
		for (y = 0; y < 272; y++)
		{
			for (x = 0; x < (480/2); x++)
			{
				if((x>=(362/2))&&(x<(480/2)))
				{
					*p2++ = 0;
				}
				else{
					*p2++ = temp[z++];
				}												
			}
			
		}
	}
	else{
		memcpy_int(LCD_FB_BASE, database, 480*272*2);
	}
*/
	
	memcpy_int((void *)LCD_FB_BASE, (void *)database, 480*272*2);
	
	/*
	for (y = 0; y < 272; y++)
	{
		for (x = 0; x < 480; x++)
		{
			
				//temp = (database[z]) | (database[1+z]<<8);
				
				//z += 2;
				temp = ((unsigned short*)database)[z++];
				*p2++ = temp;			
			
		}
			
	}
	*/
}



