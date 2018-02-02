/*
 *作者：BlueBlue
 *
 */
 
 
#define  WTCON   0xE2700000
#define  SVC_STACK  0xD0037D80
#define  IRQ_STACK  0xd0037f80
.global _start
.global IRQ_handle
_start:

	//关看门狗
	ldr r0, =WTCON
	ldr r1, =0x0
	str r1, [r0]
	
	//初始化时钟
	bl clock_init     //这是c文件函数，汇编直接调用； 另外此函数没有用变量，所以
	                  //可以放在svc栈之前
	
	//设置svc栈
	ldr sp, =SVC_STACK
	//str r0, sp
	
	//开关icache
	mrc p15, 0, r0, c1, c0, 0;
	//bic r0, r0, #(1<<12)
	orr r0, r0, #(1<<12)        // bit12 置1 开icache
	mcr p15, 0, r0, c1, c0, 0;  
	
	bl main
	
	b.
	
IRQ_handle:
    ldr sp, =IRQ_STACK
	//保存现场 是将寄存器的值保存到 sp 堆里 ；  
	sub lr, lr, #4
	
	stmfd sp!, {r0-r12, lr}
	bl irq_handler
	//恢复现场
	ldmfd sp!, {r0-r12, pc}^
	
    

	