#include <stdio.h>
#include "os_core.h"
#define STK_SIZE	1024
/* 创建os_tcbtbl 空数组，以后备用 */
OS_TCB os_tcbtbl[64];
OS_TCB idle_header;	/* 头节点 */
OS_TCB tcb_header;	/* 头节点 */

void os_list_add_node(OS_TCB *header,OS_TCB * list)
{
    OS_TCB *cur_node = &header;
    /*遍历链表*/
    for( ;cur_node; cur_node = cur_node->OSTCBNext)
    {
	    /*找到最后一个节点*/
	    if(cur_node->OSTCBNext == NULL)
	    {
		cur_node->OSTCBNext = list;
		break;		
	    }
    }
}

void os_list_delet_node(OS_TCB *header,OS_TCB * list)
{
    OS_TCB *cur_node = &header;	/* 头指针 */
   /*遍历链表*/ 
    for(; cur_node; cur_node = cur_node->OSTCBNext)
    {
	    /*找到当前节点的下一个节点等于list的节点*/
	    if(cur_node->OSTCBNext == list)
	    {
	    	cur_node->OSTCBNext = cur_node->OSTCBNext->OSTCBNext;
		break;
	    }
    }

}
/* 初始化TCB空闲链表 */
void init_os_idle_tcbtbl(void)
{
    int i;
    for(i = 0; i < 64; i++)
    {
        os_list_add_node(&idle_header,&os_tcbtbl[i]);
    }
}

void add_tcbtbl(OS_TCB *list)
{
	os_list_add_node(&tcb_header, list);  /* 将该节点添加到该链表 */
	
	os_list_delet_node(&idle_header, list);  /* 将该节点从空闲TCB链表中删除 */
}

void init_tcb()
{

}

void os_init(void)
{
	init_os_idle_tcbtbl();

}

int os_tcb_init(unsigned char prio, char *ptos, unsigned stk_size)
{
	if(os_tcbtbl[prio].OSTCBPrio)
			return -1;
	os_tcbtbl[prio].OSTCBStkPtr = ptos;
	os_tcbtbl[prio].SKT_SIZE	= stk_size;
	os_tcbtbl[prio].OSTCBPrio	= prio;
	os_tcbtbl[prio].OSTCBStat	= READY;
}

void init_stk()
{
	
}
int os_task_create(void(*task)(void *pd), void *pdata, char *ptos, unsigned char prio )
{
	if(prio > 64)
		return -1;
	
	os_tcb_init(prio, ptos, STK_SIZE);
	add_tcbtbl(&os_tcbtbl[prio]);
	init_stk();
}




















