#include <stddef.h>
#include <stdint.h>

#include "interrupts.h"
#include "keyboard_map.h"
#include "terminal.h"
#include "print.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

extern char inb(uint8_t port);
extern void outb(uint8_t port, unsigned char data);
extern void keyboard_handler(void);
extern void load_idt(unsigned long *);



void gen_int_handler()
{
  printk("Caught some kinda exception\n");
}


void gpf_handler()
{
  printk("gpf\n");
}

void gen_int()
{
  int i;
  unsigned long handler = (unsigned long)gen_int_handler;
  
  
  for(i = 0; i <= 15; i++)
    {
      IDT[i].offset_lowerbits = handler & 0xffff;
      IDT[i].sel = 0x08;
      IDT[i].zero = 0;
      IDT[i].type = 0x8e;
      IDT[i].offset_upperbits = (handler & 0xffff0000) >> 16;
    }    

  IDT[13].offset_lowerbits = (unsigned long)gpf_handler & 0xffff;
  IDT[13].offset_upperbits = ((unsigned long)gpf_handler & 0xffff0000) >> 16;
  
}


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

  outb(0x20, 0x11);
  outb(0xA0, 0x11);

  outb(0x21, 0x20);
  outb(0xA1, 0x28);

  outb(0x21, 0x00);
  outb(0xA1, 0x00);

  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  outb(0x21, 0xff);
  outb(0xA1, 0xff);

  idt_addr = (unsigned long)IDT;
  
  idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_addr & 0xffff) << 16);
  idt_ptr[1] = idt_addr >> 16;

  load_idt(idt_ptr);

}


void keyboard_handler_main(void)
{
  unsigned char status;
  char keycode;

  outb(0x20, 0x20);

  status = inb(KEYBOARD_STATUS_PORT);

  if (status & 0x01)
    {
      keycode = inb(KEYBOARD_DATA_PORT);

      if (keycode < 0)
	return;
      
      term_putchar(keyboard_map[keycode]);
    }

  return;
}


void kb_init(void)
{
  outb(0x21, 0xFD);
}

