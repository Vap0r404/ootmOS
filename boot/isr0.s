	.global isr0
	.extern fault_handler

	.section .text

isr0:
	cli
	pushl $0          # dummy error code
	pushl $0          # interrupt number 0 (divide by zero)
	call fault_handler
	add $8, %esp
	sti
	iret

