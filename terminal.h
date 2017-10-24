#define TERM_WIDTH 80
#define TERM_HEIGHT 25
#define TERM_TOP 0xb8000

void term_scroll(void);
void term_clrscr(void);
void term_putchar(char);
void term_putstr(char *);
uint16_t termchar(unsigned char, uint8_t);
size_t strlen(char *);




