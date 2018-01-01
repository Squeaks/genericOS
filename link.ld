OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

PHYS_BASE = 0x00100000;
REAL_BASE = 0x00001000;

SECTIONS
{

  . = PHYS_BASE;

  .multiboot : {
      *(.multiboot);
  }

  __physreal_diff = . - REAL_BASE;
  
  .realmode REAL_BASE : AT(ADDR(.realmode) + __physreal_diff) {

        __realmode_vma_start = .;

        /* LOADADDR is the LMA of the specified section */
        __realmode_lma_start = LOADADDR(.realmode);

       *(.text.realmode);
       *(.data.realmode);
  }


  . = ALIGN(4);
  __realmode_vma_end = .;
  __realmode_secsize   = ((__realmode_vma_end)-(__realmode_vma_start));
  __realmode_secsize_l = __realmode_secsize>>2;
  __realmode_lma_end   = __realmode_vma_start + __physreal_diff + __realmode_secsize;

  . += __physreal_diff;

  .text ALIGN(4K) : AT(ADDR(.text))
  {
      *(.text);
  }

  .data BLOCK(4K) : ALIGN(4K)
  {
      *(.data);
  }

  .data BLOCK(4K) : ALIGN(4K)
  {
      *(.rodata);
  }

  .eh_frame :
  {
      *(.eh_frame*)
  }

  .bss BLOCK(4K) : ALIGN(4K)
  {
      *(.bss);
  }


}
