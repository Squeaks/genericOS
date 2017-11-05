#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include "terminal.h"

static int internal_printk(char *, char *, va_list);

#if 0
/* Need to null term this? */
int s_printk(char *fmt, char *str)
{
  term_putstr(str);

  return strlen(str);
}
#endif

static int d_printk(char *buf, int val, int base)
{
  static const char digits[16] = "0123456789ABCDEF"; 

  char tmp[32];
  int i = 0, len = 0;

  if (val == 0)
    {
      tmp[i] = '0';
      return 1;
    }
  else
    {
      if (val < 0)
	{
	  tmp[i++] = '-';
	  len++;
	}	
      while (val != 0)
	{
	  tmp[i++] = digits[val % base];
	  val /= base;
	  len++;
	}
    }


  for (i--; i >= 0; i--)
    {
      *buf++ = tmp[i];
    }  

  return len;
}


static int internal_printk(char *buf, char *fmt, va_list args)
{
  int len = 0, num_len = 0;
  char *str, *buf_head;

  buf_head = buf;
  size_t i, j;
  
  for (i = 0; fmt[i] != '\0'; i++)
    {
      if (fmt[i] != '%')
	{
	  *buf++ = fmt[i];
	  len++;
	  continue;
	}
      else
	{
	  switch (fmt[i + 1])
	    {
	    case 'd':
	      num_len = d_printk(buf, va_arg(args, int), 10);
	      buf += num_len;
	      len += num_len;
	      i++;
	      break;
	    case 'x':
	    case 'p':
	      /* Set 0x here so it doesn't get reversed in d_printk */
	      *buf++ = '0';
	      *buf++ = 'x';
	      len += 2;
	      
	      num_len = d_printk(buf, va_arg(args, int), 16);
	      buf += num_len;
	      len += num_len;
	      i++;
	      break;
	    case 's':
	      str = va_arg(args, char *);
	      for (j = 0; str[j] != '\0'; j++)
		{
		  *buf++ = str[j];
		  len++;
		}
	      i++;
	      break;
	    default:
	      break;
	    }
	}
    }      

  term_putstr(buf_head);

  return len;
}

int printk(char *fmt, ...)
{
  uint16_t len = 0;
  va_list args;

  char buf[512] = { 0 };
  
  if (!fmt)
    return -1;

  va_start(args, fmt);
  len = internal_printk(buf, fmt, args);
  va_end(args);

  return len;
}

