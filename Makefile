
CLAGS="-g"

kernel: kernel.o
	ld -m elf_i386 -T link.ld -o kernel boot.o kc.o terminal.o print.o

boot.o:
	nasm -f elf32 boot.asm -o boot.o

terminal.o:
	gcc -m32 -c terminal.c -o terminal.o -ffreestanding -O2 -nostdlib ${CFLAGS}

print.o:
	gcc -m32 -c print.c -o print.o -ffreestanding -O2 -nostdlib -Wall ${CFLAGS}

kernel.o: boot.o terminal.o print.o
	gcc -m32 -c kernel.c -o kc.o -ffreestanding -O2 -nostdlib ${CFLAGS}

run: kernel
	qemu-system-i386 -kernel kernel -curses

runiso:
	qemu-system-i386 -cdrom genos.iso -curses

iso:
	../grub/grub-mkrescue -o genos.iso iso

clean:
	rm -f kernel *.o genos.iso

start: clean run
