typedef struct{
  uint32_t eax;
  union {
    uint16_t anop;
    uint16_t ax;
    union {
      uint8_t ah;
      uint8_t al;
    };
  };
  uint32_t ecx;
  union {
    uint16_t cnop;
    uint16_t cx;
    union {
      uint8_t ch;
      uint8_t cl;
    };
  };
  uint32_t edx;
  union {
    uint16_t dnop;
    uint16_t dx;
    union {
      uint8_t dh;
      uint8_t dl;
    };
  };
  uint32_t ebx;
  union {
    uint16_t bnop;
    uint16_t bx;
    union {
      uint8_t bh;
      uint8_t bl;
    };
  };
  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;
} reg;

reg getreg(void);
void printreg(reg);
