	bits 32

	MBALIGN equ 1<<0
	MEMINFO equ 1<<1
	FLAGS equ MBALIGN | MEMINFO
	MAGIC equ 0x1BADB002
	CHECKSUM equ -(MAGIC + FLAGS)


	section .multiboot
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

	section .text
	global start:function (start.end - start)

start:
	mov esp, stack_top
	extern kmain
	call kmain
	cli
	.hang	hlt
	jmp .hang
	.end:
read_port:
	mov edx, [esp + 4]
	in al, dx
	ret
write_port:
	mov edx, [esp + 4]
	mov al, [esp + 4 + 4]
	out  dx, al
	ret
	
	section .bss
	align 16
stack_bottom:	
	resb 8192
stack_top:	
