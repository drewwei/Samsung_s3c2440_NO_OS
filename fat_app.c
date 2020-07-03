#include "fatfs/ff.h"
#include "stdio.h"
BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */

int fatfs_app(void)
{
    FATFS fs;           /* Filesystem object */
    FIL fil;            /* File object */
    FRESULT res;        /* API result code */
    UINT bw;            /* Bytes written */
    UINT br;            /* Bytes read */
    printf("fatfs_app\r\n");
    
    /* Gives a work area to the default drive */
    res = f_mount(&fs, "0:", 1);
    if (res) 
    {
        printf("f_mount fail,res:%d\r\n",res);
        return res;
    }
    /* Format the default drive with default parameters */
	res = f_mkfs("0:", 0, work, sizeof(work));
    if (res) 
    {
        printf("f_mkfs fail,res:%d\r\n",res);
        return res;
    }
    

    /* Create a file as new */
    res = f_open(&fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
    if (res) 
    {
        printf("f_open fail,res:%d\r\n",res);
        return res;
    }

    /* Write a message */
    res=f_write(&fil, "Hello, World!\r\n", 15, &bw);
    //if (bw != 15) ...
    if (res) 
    {
        printf("f_write fail,res:%d\r\n",res);
        return res;
    }
    /* Close the file */
    f_close(&fil);

    /* Unregister work area */
    //f_mount(0, "1", 0);

    res = f_open(&fil, "hello.txt", FA_READ);
    if (res) 
    {
        printf("f_open READ fail\r\n");
        return res;
    }

   res = f_read(&fil, work, 15, &br);
    if (res) 
    {
        printf("f_read READ fail\r\n");
        return res;
    }
   printf("br:%d\r\n", br);

   f_close(&fil);

   f_mount(0, "0:", 0);

   return 0;
}
