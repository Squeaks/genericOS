#include <stdint.h>

#include "register.h"
#include "print.h"

struct e820_entry {
  uint64_t base;
  uint64_t region_len;
  uint32_t type;
  uint32_t acpi_ext;
};

static char *e820_type[2];

struct e820_entry *e820_map;


#if 0
__attribute__ ((section(".text.realmode")))
void do_e820()
{
  struct e820_entry entry;
  uint32_t count = 0;
  int entries = 0, sig, bytes;

  __asm__ __volatile__ ("int $0x15" :
	   "=a"(sig), "=c"(bytes), "=b"(count)
			: "a"(0x0000E820), "b"(count), "c"(24), "D"(&entry));

}
#endif

void print_e820_map(void *mem_map)
{
  //Should actually figure out the count somehow..

  e820_type[0] = "0 NONE";
  e820_type[1] = "1 RAM";
  e820_type[2] = "2 RESERVED";
  
  struct e820_entry *map = mem_map;
  char *ent_end;
  int i;

  for (i = 0; i < 5; i++)
    {
      printk("%d: %x - ", i, map->base);
      ent_end = map->base + map->region_len;
      printk("%x = %s\n", ent_end, e820_type[map->type]);

      map = (char *)map + sizeof(struct e820_entry);
    }
  printk("e820 done\n");
}
