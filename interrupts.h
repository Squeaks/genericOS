#ifndef INTERRUPTS_DEFINED
#define INTERRUPTS_DEFINED

void idt_init(void);
void kb_init(void);
void gen_int(void);


struct IDT_entry {
  unsigned short int offset_lowerbits;
  unsigned short int sel;
  unsigned char zero;
  unsigned char type;
  unsigned short int offset_upperbits;
};

#define IDT_SIZE 256
struct IDT_entry IDT[IDT_SIZE];

#endif
