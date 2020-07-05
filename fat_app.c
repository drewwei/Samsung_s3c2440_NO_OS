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
    
    res = f_mount(&fs, "0:", 1);
    if(res)
    {
        /* Format the default drive with default parameters */
        res = f_mkfs("0:", 0, work, sizeof(work));
        if (res) 
        {
            printf("f_mkfs fail,res:%d\r\n",res);
            return res;
        }
        /* Gives a work area to the default drive */
        res = f_mount(&fs, "0:", 1);
        if (res) 
        {
            printf("f_mount fail,res:%d\r\n",res);
            return res;
        }

        printf("f_mount fail first\r\n");
    }


#if  1    
    /* Create a file as new */
    res = f_open(&fil, "0:hello", FA_WRITE|FA_CREATE_ALWAYS);
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
    printf("bw:%d\r\n", bw);
    /* Close the file */
    res = f_close(&fil);
    if(res)
    {
        printf("f_close fail res:%d\r\n",res);
    }
 
#endif
    /* Unregister work area */
    //f_mount(0, "1", 0);

    int data[15];
    res = f_open(&fil, "0:hello", FA_READ|FA_CREATE_ALWAYS);
    if (res) 
    {
        printf("f_open READ fail res:%d\r\n",res);
        return res;
    }

   res = f_read(&fil, data, 15, &br);
    if (res) 
    {
        printf("f_read READ fail res:%d\r\n",res);
        return res;
    }
   printf("br:%d\r\n", br); 
   printf("work:%s\r\n", data);

   f_close(&fil);

   //f_mount(0, "0:", 0);

   return 0;
}
