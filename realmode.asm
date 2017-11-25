	bits 16
	global PM16

idtR:			
	dw 0x3ff
	dw 0

	
PM16:
	cli
	mov eax, cr0
	and eax, 0xFFFE
	mov cr0, eax

	db 0x66
	mov ax, 0x20
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
	sti
	
	mov eax, 0x0000E820
	mov ebx, 0
	mov ecx, 24
	db 0x67
	mov edx, 0x534D4150
	
	int 0x15

	mov eax, cr0
	or eax, 1
	mov cr0, eax

	ret
	
realMode:
	db 0x66
	mov ax, 0
	db 0x66
	mov ds, ax
	db 0x66
	mov fs, ax
	db 0x66
	mov gs, ax
	db 0x66
	mov ss, ax
	db 0x66
	mov es, ax
	db 0x66
	mov sp, 0x8000
	db 0x66
	lidt [0x0]
	
	sti
	ret
