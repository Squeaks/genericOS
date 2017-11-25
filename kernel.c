#include <stddef.h>
#include <stdint.h>

#include "terminal.h"
#include "print.h"
#include "serial.h"
#include "register.h"
#include "a20.h"

#include "interrupts.h"
#include "gdt.h"

extern void load_gdt(uint64_t *);
extern void e820(void);

extern void PM16(void);

uint64_t GDT[4];
uint64_t GDT_16[2];

uint64_t
create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
  uint64_t descriptor;

  // Create the high 32 bit segment
  descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
  descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
  descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
  descriptor |=  base        & 0xFF000000;         // set base bits 31:24

  // Shift by 32 to allow for low part of segment
  descriptor <<= 32;

  // Create the low 32 bit segment
  descriptor |= base  << 16;                       // set base bits 15:0
  descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0

  //printf("0x%.16llX\n", descriptor);
  return descriptor;
}

void set_gdt()
{
  GDT[0] = create_descriptor(0, 0, 0);
  GDT[1] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
  GDT[2] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
  GDT[3] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
  GDT[4] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));

  load_gdt(GDT);
}

void set_gdt_16()
{
  GDT_16[0] = create_descriptor(0, 0, 0);
  GDT_16[1] = create_descriptor(0, 0x0000FFFFF, (GDT_CODE_PL0_16));
  GDT_16[2] = create_descriptor(0, 0x0000FFFFF, (GDT_DATA_PL0_16));
  //  GDT[3] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3_16));
  //  GDT[4] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3_16));
  printk("set1\n");
  load_gdt(GDT_16);
}


void to_realmode()
{
  printk("to real\n");
  //  asm volatile ("cli;" : : : );
  printk("to real2\n");
  set_gdt_16();
  printk("to real3\n");
  PM16();
  printk("to real4\n");

}


void is_protected()
{
  uint32_t val;
  asm volatile ("mov %%cr0, %0;" : "=r" (val) : );

  printk("mode:%x\n", val);

  #if 0
  if (val[0])
    {
      printk("Is protected\n");
    }
  else
    {
      printk("In Realmode\n");
    }
  #endif
}

void kmain(void)
{

  char asd[3];
  char zxc[4];
  char sdf[123];
  
  term_clrscr();

  idt_init();
  gen_int();
  serial_init();
  kb_init();

  is_protected();
  
  char *bootmsg = "Starting genericOS";
  printk("%s\n", bootmsg);

  reg reg = getreg();
  printreg(reg);

  test_a20();

  to_realmode();
  
  e820();
  
  while(1);
}
    
