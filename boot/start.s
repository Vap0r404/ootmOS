	.global start
	.extern kmain

	.section .text

start:
	cli

	# Multiboot gives us a 32-bit environment; stay in 32-bit for now.
	# Set up a basic stack.
	mov $stack_top, %esp

	# EAX = Multiboot magic, EBX = multiboot_info* (per Multiboot1 spec).
	# Pass them as (uint32_t magic, uint32_t info_addr) arguments to kmain.
	push %ebx
	push %eax
	call kmain

halt:
	hlt
	jmp halt

	.bss
	.align 16
stack_bottom:
	.skip 16384
stack_top:

