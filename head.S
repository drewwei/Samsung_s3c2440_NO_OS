#include "s3c24xx.h"
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))
#define S3C2440_MPLL_400MHZ     ((0x5c<<12)|(0x01<<4)|(0x01))
#define MEM_CTL_BASE    0x48000000
#define FCLOCK_HCLOCK_PCLOCK_1_4_8		0x5
#define FCLOCK_HCLOCK_PCLOCK_1_4_4		0x4
#define    SVC                0xd3
#define    IRQ                0xd2

.extern     main
.global _start
_start:						/* 建立异常向量表 */
	b reset

@ 0x04: 未定义指令中止模式的向量地址
HandleUndef:
    b   HandleUndef 
 
@ 0x08: 管理模式的向量地址，通过SWI指令进入此模式
HandleSWI:
    b   HandleSWI

@ 0x0c: 指令预取终止导致的异常的向量地址
HandlePrefetchAbort:
    b   HandlePrefetchAbort

@ 0x10: 数据访问终止导致的异常的向量地址
HandleDataAbort:
    b   HandleDataAbort

@ 0x14: 保留
HandleNotUsed:
    b   HandleNotUsed

@ 0x18: 中断模式的向量地址
    b   HandleIRQ

@ 0x1c: 快中断模式的向量地址
HandleFIQ:
    b   HandleFIQ

.balignl 16,0xdeadbeef

reset:	
	ldr r0, =0x53000000		@关看门狗
	mov r1, #0
	str r1, [r0]
	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300			@ clear bits 13, 9:8 (--V- --RS)
	bic	r0, r0, #0x00000087			@ clear bits 7, 2:0 (B--- -CAM)
	orr	r0, r0, #0x00000002			@ set bit 2 (A) Align
	orr	r0, r0, #0x00001000			@ set bit 12 (I) I-Cache
	mcr	p15, 0, r0, c1, c0, 0
	
	ldr r0, =MPLLCON		@设置pll
	ldr r1, =S3C2440_MPLL_400MHZ
	str r1, [r0] 
	
	ldr r0, =CLKDIVN		@设置分频
	mov r1, #FCLOCK_HCLOCK_PCLOCK_1_4_8
	str r1, [r0]
	
	mrc p15,0,r0,c1,c0,0	@进入异步模式
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0
	
	

	@@初始化sdram@@
	mov  r1,     #MEM_CTL_BASE       @ 存储控制器的13个寄存器的开始地址
    adrl r2, mem_cfg_val        	 @ 这13个值的起始存储地址
    add  r3,     r1, #52             @ 13*4 = 54
1:  
    ldr r4,     [r2], #4            @ 读取设置值，并让r2加4
    str r4,     [r1], #4            @ 将此值写入寄存器，并让r1加4
    cmp r1,     r3                  @ 判断是否设置完所有13个寄存器
    bne 1b                          @ 若没有写成，继续
    
	

	ldr sp, =0x34000000         		@ 设置管理模式栈指针
	
	@@初始化nand@@
	
	bl nand_init
	
	@@从nandflash中拷贝代码到sdram
	
	ldr r0, =0x30000000
	mov r1, #0
	ldr r2, =(__bss_end - 0x30000000)
	bl nand_read
	
	@@清bss段
	
	mov r0, #0
	ldr r1, =__bss_start
	ldr r2, =__bss_end
2:
	str r0, [r1], #4
	cmp r1, r2
	bne 2b
	
	@ldr lr, =run_in_sdram
	@@重定位
	//ldr pc, =run_in_sdram
	
	
	
/*******************************************	
	ldr r0, =0x56000050		@@@ 点亮led测试用
	ldr r1, [r0]
	bic r1, r1, #(3<<12)
	orr r1, r1, #(1<<12)
	str r1, [r0]
	ldr r0, =0x56000054
	ldr r1, [r0]	
	@bic r1, r1, #(1<<6)
	orr r1, r1, #(1<<6)
	str r1, [r0]
*********************************************/

run_in_sdram:	
	
	
	//msr cpsr_c, #0x53     	@ 设置I-bit=0，开IRQ中断,进入管理
	
	msr cpsr_c, #IRQ        	@ 进入中断模式
	ldr sp, =0x33000000 		@ 设置中断模式栈指针  @此处曾经少写一个0导致中断后程序从新启动
	msr cpsr_c, #SVC        	@ 进入管理模式
	ldr sp, =0x34000000
	
	
	ldr lr, =halt
	ldr pc, =main		/* 绝对跳转，跳到sdram中 */
halt:
	b halt
	
HandleIRQ:
		sub lr, lr, #4
		stmdb sp!, {r0-r12,lr}   @保存现场		
		ldr lr, =int_return		
		ldr pc, =IrqHandler	
int_return:		
		ldmia sp!, {r0-r12, pc}^ @恢复现场

.align 4
mem_cfg_val:
    @ 存储控制器13个寄存器的设置值
    .long   0x22011110      @ BWSCON
    .long   0x00000700      @ BANKCON0
    .long   0x00000700      @ BANKCON1
    .long   0x00000700      @ BANKCON2
    .long   0x00000700      @ BANKCON3  
    .long   0x00000700      @ BANKCON4
    .long   0x00000700      @ BANKCON5
    .long   0x00018005      @ BANKCON6
    .long   0x00018005      @ BANKCON7
    .long   0x008C07A3      @ REFRESH
    .long   0x000000B1      @ BANKSIZE
    .long   0x00000030      @ MRSRB6
    .long   0x00000030      @ MRSRB7

