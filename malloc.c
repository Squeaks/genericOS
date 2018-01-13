#include <stdint.h>
#include <stddef.h>
#include "print.h"
#include "e820.h"

#include "malloc.h"

extern struct e820_entry *e820_map;

typedef struct mem_entry_t {
  struct mem_entry_t *next;
  // Could make this a bitfield or something
  uint8_t is_tail;
  uint8_t is_used;
  void *addr;
  uint32_t size;
} mem_entry;

//typedef struct mem_entry_t mem_entry;

static mem_entry *mem_head;
static void *mem_start;
static void *mem_end;

void mem_init()
{
  mem_head = NULL;
  // No! Wrong! Bad! Blind selection is stupid
  // + 1M because kernel starts at 1M.
  mem_start = (void *)e820_map[3].base + 0x100000;
  mem_end = (void *)e820_map[3].base + e820_map[3].region_len;
  printk("Available memory for malloc: %dKB\n", (e820_map[3].region_len / 1024));
}


void *malloc(uint32_t size)
{
  void *ret;

  if (!mem_head)
    {
      mem_head = mem_start;
      mem_head->size = size;

      /* |mem_entry|size|mem_entry|size| */
      mem_head->next = (mem_entry*)((char *)mem_head + size + sizeof(mem_entry));

      if ((char *)mem_head->next >= (char *)mem_end)
	{
	  printk("ENOMEM. We've reached the end of mem region\n");
	  return NULL;
	}

      mem_head->addr = (void *)((char *)mem_head + sizeof(mem_entry));
      mem_head->is_tail = 1;
      ret = mem_head->addr;
    }
  else
    {
      mem_entry *tmp;

      tmp = mem_head;

      while (!tmp->is_tail)
	{
	  tmp = tmp->next;
	}

      tmp->is_tail = 0;
      tmp = tmp->next;
      tmp->size = size;
      tmp->next = (mem_entry *)((char *)tmp + size + sizeof(mem_entry));

      if ((char *)tmp->next >= (char *)mem_end)
	{
	  printk("ENOMEM. We've reached the end of mem region\n");
	  return NULL;
	}

      tmp->addr = (void *)((char *)tmp + sizeof(mem_entry));
      tmp->is_tail = 1;
      ret = tmp->addr;
    }

  return ret;
}

/* Doesn't compact at all, but will reset the head if the list empties */
void free(void *addr)
{
  mem_entry *tmp, *prev;

  tmp = mem_head;

  while (tmp->addr != addr && !tmp->is_tail)
    {
      prev = tmp;
      tmp = tmp->next;
    }

  if (tmp->is_tail && tmp->addr != addr)
    {
      printk("Couldn't find entry to free\n");
      return;
    }

  if (tmp->addr == mem_head->addr)
    {
      mem_head = tmp->next;
      printk("Freed head. New head is at %x\n", mem_head);
    }

  if (!mem_head->size)
    {
      printk("Malloc list is empty. Resetting head\n");
      mem_head = NULL;
      return;
    }
  prev->next = tmp->next;

  /* Zero the item because our empty head check is stupid */
  memzero(tmp, sizeof(mem_entry) + tmp->size);
}

void memzero(void *p, uint32_t size)
{
  int i;
  char *ptr = p;
  for (i = 0; i < size; i++)
    {
      ptr[i] = 0;
    }
}
