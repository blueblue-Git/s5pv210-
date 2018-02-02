#include "main.h"
#define PS_HOLD_CONTROL     0xE010E81C
#define rPS_HOLD_CONTROL    *((volatile unsigned int *)PS_HOLD_CONTROL)

#define KEY_EINT2    NUM_EINT2   
#define KEY_EINT3    NUM_EINT3
#define KEY_EINT16_19  NUM_EINT16_31
#define UART0_INT     NUM_UART0
int main(void)
{
	uart_init();
    led_blink();
	
	exit_init(); //从初始化外部中断
	
	system_interrupt();//初始化终端控制器
	
	intc_setvectaddr(KEY_EINT2,isr_eint2);
	intc_setvectaddr(KEY_EINT3,isr_eint3);
	intc_setvectaddr(KEY_EINT16_19,isr_eint16171819);
	
	intc_setvectaddr(UART0_INT,isr_uart0); 
	
	intc_enable(KEY_EINT2);
	intc_enable(KEY_EINT3);
	intc_enable(KEY_EINT16_19);
	
	intc_enable(UART0_INT);
	
	
	rPS_HOLD_CONTROL = 0x00005301;
	
    print(" \n OK \n");	
	while(1);
	
	return 0;
}