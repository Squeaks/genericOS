#include <stddef.h>
#include <stdint.h>

#include "terminal.h"

size_t TERM_POSX = 0;
size_t TERM_POSY = 0;

void term_scroll()
{
  uint16_t *term = (uint16_t *)TERM_TOP;
  uint8_t i, j;
  
  for (i = 0; i < TERM_WIDTH; i++)
    {
      for (j = 1; j < TERM_HEIGHT; j++)
	{
	  term[((j - 1) * TERM_WIDTH) + i] = term[(j * TERM_WIDTH) + i];
	  /* Clear bottom row */
	  if (j == TERM_HEIGHT - 1)
	    term[(j * TERM_WIDTH) + i] = termchar(' ', 0x07);
	}
    }
}  

void term_clrscr()
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


void term_putchar(char c)
{
  uint16_t *term = (uint16_t *)TERM_TOP;

  if (c == '\n')
    {
      TERM_POSX = 0;
      TERM_POSY++;
      if (TERM_POSY >= TERM_HEIGHT)
	{
	  TERM_POSY = 24;
	  term_scroll();
	}
      return;
    }

  term[(TERM_POSY * TERM_WIDTH + TERM_POSX)] = termchar(c, 0x07);
  TERM_POSX++;

  if (TERM_POSX == TERM_WIDTH)
    {
      TERM_POSX = 0;
      TERM_POSY++;
      if (TERM_POSY >= TERM_HEIGHT)
	{
	  TERM_POSY = 24;
	  term_scroll();
	}
    }
}

void term_putstr(char *str)
{
  uint16_t *term = (uint16_t *)0xb8000;
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

size_t strlen(char *str)
{
  size_t i = 0;
  while (str[i] != '\0')
    {
      i++;
    }
  return i;
}
