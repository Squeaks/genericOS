CFLAGS=-g -O0

kernel: kernel.o
	ld -m elf_i386 -T link.ld -o kernel boot.o kc.o terminal.o print.o serial.o register.o a20.o \
	interrupts.o mem.o realmode.o malloc.o

interrupts.o:
	gcc -m32 -c interrupts.c -o interrupts.o -ffreestanding -nostdlib ${CFLAGS}

boot.o:
	nasm -f elf32 boot.asm -o boot.o

realmode.o:
	nasm -f elf32 realmode.asm -o realmode.o

terminal.o:
	gcc -m32 -c terminal.c -o terminal.o -ffreestanding  -nostdlib ${CFLAGS}

print.o:
	gcc -m32 -c print.c -o print.o -ffreestanding -nostdlib -Wall ${CFLAGS}

serial.o:
	gcc -m32 -c serial.c -o serial.o -ffreestanding -nostdlib -Wall ${CFLAGS}

register.o:
	gcc -m32 -c register.c -o register.o -ffreestanding -nostdlib -Wall ${CFLAGS}

a20.o:
	gcc -m32 -c a20.c -o a20.o -ffreestanding -nostdlib -Wall ${CFLAGS}

mem.o:
	gcc -m32 -c mem.c -o mem.o -ffreestanding -O0 -nostdlib -Wall ${CFLAGS}

malloc.o:
	gcc -m32 -c malloc.c -o malloc.o -ffreestanding -O0 -nostdlib -Wall ${CFLAGS}

kernel.o: boot.o terminal.o print.o serial.o register.o a20.o interrupts.o mem.o realmode.o malloc.o
	gcc -m32 -c kernel.c -o kc.o -ffreestanding -nostdlib ${CFLAGS}



run: kernel
	qemu-system-i386 -kernel kernel -curses -serial file:logfile.out -cpu 486 2> stderr.out

debug: kernel
	qemu-system-i386 -kernel kernel -curses -serial file:logfile.out -s -S -cpu 486

gdb:
	gdb kernel -ex "target remote localhost:1234"

runiso:
	qemu-system-i386 -cdrom genos.iso -curses

iso:
	../grub/grub-mkrescue -o genos.iso iso

clean:
	rm -f kernel *.o genos.iso

start: clean run

sea:
	qemu-system-i386 -bios bios.bin -kernel kernel -curses -serial file:logfile.out -chardev stdio,id=seabios -device isa-debugcon,iobase=0x402,chardev=seabios 2> stderr.log
