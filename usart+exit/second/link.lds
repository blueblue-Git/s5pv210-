SECTIONS
{
	.=0xd0020010
	.text:{
		start.o
		*(.test)
	}
	.data:{
	    *(.data)
	}
	bss_start = .;
	.bss:{
		*(.bss)
	}
	bss_end = .;
}