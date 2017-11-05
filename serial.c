#include <stdint.h>
#include "serial.h"

#define COM1 0x3F8

extern char inw(uint16_t port);
extern void outw(uint16_t port, unsigned char data);

uint8_t serial_enabled = 0;

int is_serial_enabled()
{
  return serial_enabled;
}

void serial_init()
{
  /* Disable interrupts */
  outw(COM1 + 1, 0);
  outw(COM1 + 3, 0x80);
  /* Set baud rate to 9600 with div 12 */
  outw(COM1, 0x0b);
  outw(COM1 + 1, 0x00);
  /* Set line control to 8N1 */
  outw(COM1 + 3, 0x03);
  /* Set 4 byte interrupt level, clear and enable FIFOs */
  outw(COM1 + 2, 0x67);
  outw(COM1 + 1, 1);
  serial_enabled = 1;
}

int poll_serial()
{
  char val;
  if (!inw(COM1 + 5))
    {
      val = inw(COM1);
      return (int)val;
    }
  return 0;
}

#if 0
int write_serial(char val)
{
  uint8_t status = 0;
  int ret = 0;
  status = inw(COM1 + 5);
  if (status & 0x5 || status & 0x6)
    {
      outw(COM1, val);
      ret = 0;
    }
  else
    //error
    ret = -1;

  return ret;
}
  
#endif

int write_serial(char val)
{
  outw(COM1, val);
  return 0;
}
