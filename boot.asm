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
	global keyboard_handler
	global inb
	global outb
	global inw
	global outw
	global load_idt

start:
	mov esp, stack_top
	extern kmain
	call kmain
	cli
	.hang	hlt
	jmp .hang
	.end:

inb:
	mov edx, [esp + 4]
	in al, dx
	ret

outb:
	mov edx, [esp + 4]
	mov al, [esp + 4 + 4]
	out dx, al
	ret

inw:
	mov edx, [esp + 4]
	in ax, dx
	ret
outw:
	mov edx, [esp + 4]
	mov ax, [esp + 8]
	out dx, ax
	ret

keyboard_handler:
	extern keyboard_handler_main
	call keyboard_handler_main
	iretd

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret

	section .bss
	align 16
stack_bottom:	
	resb 8192
stack_top:	
