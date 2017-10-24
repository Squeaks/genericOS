#include <stddef.h>
#include <stdint.h>

#include "terminal.h"


/* Only supports a single format */
int s_printk(char *fmt, char *str)
{
  if (!fmt)
    return 1;

  if (fmt[0] != '%')
    return 1;

  switch (fmt[1])
    {
    case 'p':
      break;
    case 'd':
      break;
    case 's':
      term_putstr(str);
      break;
    default:
      return 1;
    }
  return 0;
}
