	CODE32 equ 0x08
	DATA32 equ 0x10
	CODE16 equ 0x18
	DATA16 equ 0x20
	
	global PM16
	global to_real_mode
	
	section .data.realmode
save_idt:
	dw 0
	dd 0
save_esp:
	dd 0
real_ivt:
	dw (256 * 4) - 1
	dd 0	
	

	bits 32
	section .text
to_real_mode:

	pushad
	pushfd
	cli
	mov [save_esp], esp
	sidt [save_idt]
	lidt [real_ivt]

	jmp CODE16:PM16 

	bits 16
	section .text.realmode
PM16:

	db 0x66
	mov ax, DATA16
	db 0x66
	mov ds, ax
	db 0x66
	mov es, ax
	db 0x66
	mov fs, ax
	db 0x66
	mov gs, ax
	db 0x66
	mov ss, ax

	mov eax, cr0
	dec eax			;clear prot mode bit
	mov cr0, eax
	
	jmp dword 0:realMode

realMode:
	db 0x66
	mov ax, 0x1000
	mov ss, ax
	xor sp, sp 		;clear sp?
	
	xor ax, ax
	db 0x66
	mov ds, ax
	db 0x66
	mov fs, ax
	db 0x66
	mov gs, ax
	db 0x66
	mov es, ax

	
	cli
	mov eax, cr0
	inc eax
	mov cr0, eax
	jmp dword CODE32:PM32

	bits 32
	section .text
PM32:	
	mov eax, DATA32
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov dword esp, [save_esp]
	lidt [save_idt]
	popfd
	popad

	mov eax, 0xdead
	;; mov ax, e820 struct
	sti
	ret

	
