#include <stddef.h>
#include <stdint.h>

#include "keyboard_map.h"

#define TERM_WIDTH 80
#define TERM_HEIGHT 25

#define TERM_TOP 0xb8000

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

size_t strlen(const char*);
uint16_t termchar(unsigned char, uint8_t);

size_t TERM_POSX = 0;
size_t TERM_POSY = 0;

extern char read_port(uint8_t port);
extern void write_port(uint8_t port, unsigned char data);
extern void keyboard_handler(void);
extern void load_idt(unsigned long *);


void term_putchar(char c)
{
  uint16_t *term = (uint16_t *)0xb8000; // + (TERM_POSY * TERM_WIDTH) + TERM_POSX);

  if (c == '\n')
    {
      TERM_POSX = 0;
      TERM_POSY++;
      return;
    }

  term[(TERM_POSY * TERM_WIDTH + TERM_POSX)] = termchar(c, 0x07);
  TERM_POSX++;
}

void term_putstr(const char *str)
{
  uint16_t *term = (uint16_t *)0xb8000; // + (TERM_POSY * TERM_WIDTH) + TERM_POSX);
  size_t i;

  for (i = 0; i < strlen(str); i++)
    {
      term_putchar(str[i]);
    }
}

uint16_t termchar(unsigned char chr, uint8_t colour)
{
  return (uint16_t) chr | (uint16_t) colour << 8;
}

size_t strlen(const char *str)
{
  size_t i = 0;
  while (str[i] != '\0')
    {
      i++;
    }
  return i;
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


void term_clrscr(void)
{
  uint16_t *term = (uint16_t *)0xb8000;
  unsigned int i, j;

  for (i = 0; i < TERM_WIDTH; i++)
    {
      for (j = 0; j < TERM_HEIGHT; j++)
	{
	  term[(j * TERM_WIDTH) + i] = termchar(' ', 0x07);
	}
    }
}

void kmain(void)
{

  term_clrscr();

  term_putstr("Kernel intro\n");

  idt_init();
  kb_init();

  while(1);
}
    
