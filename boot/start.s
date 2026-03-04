	.global start
	.extern kmain

	.section .text

start:
	cli

	# Multiboot gives us a 32-bit environment; stay in 32-bit for now.
	# Set up a basic stack.
	mov $stack_top, %esp

	# Pass Multiboot info to kmain if desired in future.
	# For now we ignore the parameters and just call kmain().
	call kmain

halt:
	hlt
	jmp halt

	.bss
	.align 16
stack_bottom:
	.skip 16384
stack_top:

