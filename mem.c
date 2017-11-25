#include <stdint.h>

#include "register.h"

struct e820_entry {
  uint64_t base;
  uint64_t region_len;
  uint32_t type;
  uint32_t acpi_ext;
};
  
extern void intcall(void);
  
void e820()
{
  struct e820_entry entry;
  reg reg;
  
//write_edi(entry);
  write_ebx(0);
  /* "SMAP" magic */
  write_edx(0x534D4150);
  write_eax(0x0000e820);
  write_ecx(24);
  
  reg = getreg();
  printreg(reg);

  intcall();
}
