#ifndef      _SO_CORE_H
#define      _SO_CORE_H

/* 5种状态 */
enum status {
    WAIT=0,     /* 等待 */
    SLEEP,      /* 睡眠 */
    READY,      /* 准备 */
    RUN,        /* 运行 */
    INT         /* 中断 */
};

/* 任务控制块 */
typedef struct os_tcb {
    char * OSTCBStkPtr;
    struct os_tcb   *OSTCBNext; 
    struct os_tcb   *OSTCBPrev;
    unsigned int    *SKT_SIZE;
    char * OSTCBStkBottom;
    unsigned int    OSTCBDly;
    enum status     OSTCBStat;
    unsigned char   OSTCBPrio;   
}OS_TCB;





















#endif