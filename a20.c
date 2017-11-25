#include <stdint.h>
#include "register.h"
#include "print.h"

extern void write_gs16(uint16_t);

int test_a20()
{
  //  uint32_t bottom_a20, top_a20, bot_val = 0, top_val = 0;
  //  uint32_t bottom_a20, top_a20;
  //  write_gs16(0xFFFF);
  uint32_t gs = read_gs();
  printk("gs: %x\n", gs);
  //  write_fs(0x0000);

  //  bottom_a20 = read_fs_full(0x1000);
  //  top_a20 = read_gs_full(0x1000);

  //  printk("bot: %x\n", bottom_a20);
  //  printk("top: %x\n", top_a20);
  
  //  asm volatile ("mov %0, %%eax;" : "=r" (bottom_a20) :);
  //  asm volatile ("mov [%0], %1;" : "=m" (bottom_a20) : "r" (bot_val) );
  return 1;
}
