void reset_exception(void)
{
	printf("reset_exception.\n");
}

void undef_exception(void)
{
	printf("undef_exception.\n");
}

void sotf_int_exception(void)
{
	printf("sotf_int_exception.\n");
}

void prefetch_exception(void)
{
	printf("prefetch_exception.\n");
}

void data_exception(void)
{
	printf("data_exception.\n");
}

void system_interrupt(void)
{
	r_exception_reset = (unsigned int)reset_exception;
	r_exception_undef = (unsigned int)undef_exception;
	r_exception_sotf_int = (unsigned int)sotf_int_exception;
	r_exception_prefetch = (unsigned int)prefetch_exception;
	r_exception_data = (unsigned int)data_exception;
	r_exception_irq = (unsigned int)IRQ_handle;
	r_exception_fiq = (unsigned int)IRQ_handle;	
	
	intc_init();
}
// 清除需要处理的中断的中断处理函数的地址
void intc_clearvectaddr(void)
{
    // VICxADDR:当前正在处理的中断的中断处理函数的地址
    VIC0ADDR = 0;
    VIC1ADDR = 0;
    VIC2ADDR = 0;
    VIC3ADDR = 0;
}
void intc_init(void)
{
	VIC0INTENCLEAR = 0xffffffff;
    VIC1INTENCLEAR = 0xffffffff;
    VIC2INTENCLEAR = 0xffffffff;
    VIC3INTENCLEAR = 0xffffffff;
	
	
	VIC0INTSELECT = 0x0;
    VIC1INTSELECT = 0x0;
    VIC2INTSELECT = 0x0;
    VIC3INTSELECT = 0x0;
	
	    // 清VICxADDR
    intc_clearvectaddr();
}
void intc_setvectaddr(unsigned long intnum,void (*handler)(void))
{
	if(intnum < 32)VIC0VECTADDR
	{
		 *((volatile unsigned long *)(VIC0VECTADDR + intnum*4)) = (unsigned)handler; 
	}
	else if( intnum < 64)
	{
		 *((volatile unsigned long *)(VIC0VECTADDR + (intnum-32)*4)) = (unsigned)handler; 
	}
	else if( intnum < 96)
	{
		 *((volatile unsigned long *)(VIC0VECTADDR + (intnum-64)*4)) = (unsigned)handler; 
	}
	else
	{
		 *((volatile unsigned long *)(VIC0VECTADDR + (intnum-96)*4)) = (unsigned)handler; 
	}
}

//使能中断  
void intc_enable(unsigned long intnum)
{
	unsigned long temp;
	// 确定intnum在哪个寄存器的哪一位
	// <32就是0～31，必然在VIC0
    if(intnum<32)
    {
        temp = VIC0INTENABLE;
        temp |= (1<<intnum);		// 如果是第一种设计则必须位操作，第二种设计可以
									// 直接写。
        VIC0INTENABLE = temp;
    }
    else if(intnum<64)
    {
        temp = VIC1INTENABLE;
        temp |= (1<<(intnum-32));
        VIC1INTENABLE = temp;
    }
    else if(intnum<96)
    {
        temp = VIC2INTENABLE;
        temp |= (1<<(intnum-64));
        VIC2INTENABLE = temp;
    }
    else if(intnum<NUM_ALL)
    {
        temp = VIC3INTENABLE;
        temp |= (1<<(intnum-96));
        VIC3INTENABLE = temp;
    }
    // NUM_ALL : enable all interrupt
    else
    {
        VIC0INTENABLE = 0xFFFFFFFF;
        VIC1INTENABLE = 0xFFFFFFFF;
        VIC2INTENABLE = 0xFFFFFFFF;
        VIC3INTENABLE = 0xFFFFFFFF;
    }	
}
//关闭中断
void intc_disable(unsigned long intnum)
{
	unsigned long temp;
	// 确定intnum在哪个寄存器的哪一位
	// <32就是0～31，必然在VIC0
    if(intnum<32)
    {
        temp = VIC0INTENABLE;
        temp |= (1<<intnum);		// 如果是第一种设计则必须位操作，第二种设计可以
									// 直接写。
        VIC0INTENABLE = temp;
    }
    else if(intnum<64)
    {
        temp = VIC1INTENABLE;
        temp |= (1<<(intnum-32));
        VIC1INTENABLE = temp;
    }
    else if(intnum<96)
    {
        temp = VIC2INTENABLE;
        temp |= (1<<(intnum-64));
        VIC2INTENABLE = temp;
    }
    else if(intnum<NUM_ALL)
    {
        temp = VIC3INTENABLE;
        temp |= (1<<(intnum-96));
        VIC3INTENABLE = temp;
    }
    // NUM_ALL : enable all interrupt
    else
    {
        VIC0INTENABLE = 0xFFFFFFFF;
        VIC1INTENABLE = 0xFFFFFFFF;
        VIC2INTENABLE = 0xFFFFFFFF;
        VIC3INTENABLE = 0xFFFFFFFF;
    }
}

unsigned long intc_getvicirqstatus(unsigned long uconotroller)
{
	if(uconotroller == 0)
		return VIC0IRQSTATUS;
	else if(uconotroller == 1)
		return VIC1IRQSTATUS;
	else if(uconotroller == 2)
		return VIC2IRQSTATUS;
	else if(uconotroller == 3)
		return VIC3IRQSTATUS;
	
	return 0;
}

//1  同一个vicaddr 的两个中断  ，先后进来 但是先进来的中断不退出 
//2  不同vicaddr 的两个中断 ，先后进来，但是先进来的中断不退出
//   找到 VICVECTPRIORITY 寄存器了  测试中并没有设置这个值！
void irq_handler(void)
{
	unsigned long 	vicaddr[4] = { VIC0ADDR,VIC1ADDR,VIC2ADDR,VIC3ADDR}
	int i = 0;
	void (*isr)(void) = NULL;
	
	for( i=0; i<4; i++)
	{
			if(intc_getvicirqstatus(i) != 0)
			{
				isr = (void (*)(void))vicaddr[i];
				break;
			}
	}
	(*isr)();
}


