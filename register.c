#include <stdint.h>

#include "register.h"
#include "print.h"

uint32_t read_eax()
{
  uint32_t val;
  asm volatile ("mov %%eax, %0;" : "=r" (val) : );
  return val;
}

uint32_t read_edx()
{
  uint32_t val;
  asm volatile ("mov %%edx, %0;" : "=r" (val) : );
  return val;
}

uint32_t read_ecx()
{
  uint32_t val;
  asm volatile ("mov %%ecx, %0;" : "=r" (val) : );
  return val;
}

uint32_t read_ebx()
{
  uint32_t val;
  asm volatile ("mov %%ebx, %0;" : "=r" (val) : );
  return val;
}

uint32_t read_esp()
{
  uint32_t val;
  asm volatile ("mov %%esp, %0;" : "=r" (val) : );
  return val;
}

uint32_t read_ebp()
{
  uint32_t val;
  asm volatile ("mov %%ebp, %0;" : "=r" (val) : );
  return val;
}

uint32_t read_esi()
{
  uint32_t val;
  asm volatile ("mov %%esi, %0;" : "=r" (val) : );
  return val;
}

uint32_t read_edi()
{
  uint32_t val;
  asm volatile ("mov %%edi, %0;" : "=r" (val) : );
  return val;
}


reg getreg()
{
  reg reg;
  reg.eax = read_eax();
  reg.edx = read_edx();
  reg.ecx = read_ecx();
  reg.ebx = read_ebx();
  reg.esp = read_esp();
  reg.ebp = read_ebp();
  reg.esi = read_esi();
  reg.edi = read_edi();

  return reg;
}

void printreg(reg reg)
{
  printk("eax: %p\n", reg.eax);
  printk("edx: %p\n", reg.edx);
  printk("ecx: %p\n", reg.ecx);
  printk("ebx: %p\n", reg.ebx);
  printk("esp: %p\n", reg.esp);
  printk("ebp: %p\n", reg.ebp);
  printk("esi: %p\n", reg.esi);
  printk("edi: %p\n", reg.edi);
}
