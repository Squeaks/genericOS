
kernel: kernel.o
	ld -m elf_i386 -T link.ld -o kernel boot.o kc.o

boot.o:
	nasm -f elf32 boot.asm -o boot.o

kernel.o: boot.o
	gcc -m32 -c kernel.c -o kc.o -ffreestanding -O2 -nostdlib

run:
	qemu-system-i386 -kernel kernel -curses

runiso:
	qemu-system-i386 -cdrom genos.iso -curses

iso:
	../grub/grub-mkrescue -o genos.iso iso

clean:
	rm -f kernel kc.o boot.o genos.iso

