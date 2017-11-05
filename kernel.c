#include <stddef.h>
#include <stdint.h>

#include "keyboard_map.h"
#include "terminal.h"
#include "print.h"

#define IDT_SIZE 256

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

struct IDT_entry {
  unsigned short int offset_lowerbits;
  unsigned short int sel;
  unsigned char zero;
  unsigned char type;
  unsigned short int offset_upperbits;
};

struct IDT_entry IDT[IDT_SIZE];

extern char read_port(uint8_t port);
extern void write_port(uint8_t port, unsigned char data);
extern void keyboard_handler(void);
extern void load_idt(unsigned long *);

void idt_init(void)
{
  unsigned long keyboard_addr;
  unsigned long idt_addr;
  unsigned long idt_ptr[2];

  keyboard_addr = (unsigned long)keyboard_handler;
  IDT[0x21].offset_lowerbits = keyboard_addr & 0xffff;
  IDT[0x21].sel = 0x08;
  IDT[0x21].zero = 0;
  IDT[0x21].type = 0x8e;
  IDT[0x21].offset_upperbits = (keyboard_addr & 0xffff0000) >> 16;

  write_port(0x20, 0x11);
  write_port(0xA0, 0x11);

  write_port(0x21, 0x20);
  write_port(0xA1, 0x28);

  write_port(0x21, 0x00);
  write_port(0xA1, 0x00);

  write_port(0x21, 0x01);
  write_port(0xA1, 0x01);

  write_port(0x21, 0xff);
  write_port(0xA1, 0xff);

  idt_addr = (unsigned long)IDT;
  
  idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_addr & 0xffff) << 16);
  idt_ptr[1] = idt_addr >> 16;

  load_idt(idt_ptr);

}

void keyboard_handler_main(void)
{
  unsigned char status;
  char keycode;

  write_port(0x20, 0x20);

  status = read_port(KEYBOARD_STATUS_PORT);

  if (status & 0x01)
    {
      keycode = read_port(KEYBOARD_DATA_PORT);

      if (keycode < 0)
	return;
      
      term_putchar(keyboard_map[keycode]);
    }

  return;
}


void kb_init(void)
{
  write_port(0x21, 0xFD);
}

void kmain(void)
{

  char asd[3];
  char zxc[4];
  char sdf[123];
  
  term_clrscr();
  char *bootmsg = "Starting genericOS";
  printk("%s\n", bootmsg);
  

  int len = printk("num %d\n", 100);
  printk("asd %d\n", 99);
  printk("zxc %d\n", 321);

  printk("len: %d\n", len);

  printk("asd buf: %p\n", asd);
  printk("sdf buf: %p\n", sdf);
  printk("zxc buf: %p\n", zxc);

  printk("kmain addr: %p\n", kmain);
  printk("write_port addr: %p\n", write_port);
  printk("read_port addr: %p\n", read_port);
  

  //  printk("len %d", len);
  //  s_printk("asda", "qweqweqweqw\n");
  
  /*
  printk("%d", (void *)1232);
  printk("%s", (void *)"\n");
  printk("%p", &asd);
  printk("%p", &asd[1]);
  printk("%x", &asd[2]);
  
  printk("%x", &asd[11]);
 
  printk("%x", &qwe);
  printk("%p", &qwe[1]);
  */
  idt_init();
  kb_init();

  while(1);
}
    
