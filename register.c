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

void write_eax(uint32_t val)
{
  asm volatile ("movl %0, %%eax;" : : "r" (val) : );
}

void write_ebx(uint32_t val)
{
  asm volatile ("movl %0, %%ebx;" : : "r" (val) : );
}

void write_ecx(uint32_t val)
{
  asm volatile ("movl %0, %%ecx;" : : "r" (val) : );
}

void write_edx(uint32_t val)
{
  asm volatile ("movl %0, %%edx;" : : "r" (val) : );
}


void write_edi(uint32_t val)
{
  asm volatile ("movl %0, %%edi;" : : "r" (val) : );
}


void write_fs(uint32_t val)
{
  asm volatile ("movl %0, %%fs;" : : "r" (val) : );
}

void write_es(uint16_t val)
{
  asm volatile ("movw %0, %%es;" : : "r" (val) : );
}  

#if 0
void write_gs16(uint16_t val)
{
  asm volatile ("movw %0, %%gs;" : : "rm" (val) : );
}
#endif

void write_gs162(uint16_t val)
{
  asm volatile ("movw %0, %%gs;" : : "rm" (val) : );
}

void write_gs32(uint32_t val)
{
  asm volatile ("movl %0, %%gs;" : : "r" (val) : );
}

uint32_t read_fs()
{
  uint32_t val;
  asm volatile ("mov %%fs, %0;" : "=r" (val) : );
  return val;
}

uint32_t read_gs()
{
  uint32_t val;
  asm volatile ("mov %%gs, %0;" : "=r" (val) : );
  return val;
}

uint32_t read_fs_full(uint32_t val)
{
  uint32_t retval = 0;
  asm volatile ("movl %%fs:(%1), %0;" : "=r" (retval) : "r" (val) : );
  return retval;
}

uint32_t read_gs_full(uint32_t val)
{
  uint32_t retval = 0;
  asm volatile ("movl %%gs:(%1), %0;" : "=r" (retval) : "r" (val) : );
  return retval;
}

#if 0 
void intcall(uint8_t val)
{
  asm volatile ("int %0;" : : "r" (val) : );
}
#endif

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
 }
