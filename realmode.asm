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

	bits 16
e820_entry:
	dw 24 * 10
		
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
	mov esp, 0xf000

 	bits 32
	db 0x66
	mov edx, 0x534D4150
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	bits 16

	mov di, e820_entry

e820_start:	
	
	mov ecx, 24
	mov eax, 0xe820
	int 0x15

	jc e820_failed
	cmp ebx, 0
	jz e820_done
	
	add di, 24
	jmp e820_start

e820_failed:
	;; 	mov e820_entry], 0xdead

e820_done:
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

	mov eax, e820_entry
	;; mov ax, e820 struct
	sti
	ret

	
