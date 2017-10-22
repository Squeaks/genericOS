#include <stddef.h>
#include <stdint.h>

#define TERM_WIDTH 80
#define TERM_HEIGHT 25

#define TERM_TOP 0xb8000

size_t strlen(const char*);
uint16_t termchar(unsigned char, uint8_t);

size_t TERM_POSX = 0;
size_t TERM_POSY = 0;

void term_putstr(const char *str)
{
  uint16_t *term = (uint16_t *)0xb8000; // + (TERM_POSY * TERM_WIDTH) + TERM_POSX);
  size_t i;

  for (i = 0; i < strlen(str); i++)
    {
      if (str[i] == '\n')
	{
	  TERM_POSX = 0;
	  TERM_POSY++;
	  continue;
	}
      term[(TERM_POSY * TERM_WIDTH + TERM_POSX)] = termchar(str[i], 0x07);
      TERM_POSX++;
      if (TERM_POSX % TERM_WIDTH == 0)
	{
	  term[i] = termchar(TERM_POSX, 0x05);
	  TERM_POSY++;
	  TERM_POSX = 0;
	}
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

void kmain(void)
{

  const char *first = "It's a kernelaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n" \
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" \
    "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq" \
    "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbqqqqqqqqqqqqq";
  uint16_t *term = (uint16_t *)0xb8000;
  unsigned int i, j;

  for (i = 0; i < TERM_WIDTH; i++)
    {
      for (j = 0; j < TERM_HEIGHT; j++)
	{
	  term[(j * TERM_WIDTH) + i] = termchar(' ', 0x07);
	}
    }


  term_putstr(first);

  return;
}
    
