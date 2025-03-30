MEMORY
{
  BOOTLOADER (rx) : ORIGIN = 0x08000000, LENGTH = 16K    /* Bootloader in Flash */
  APP (rx)        : ORIGIN = 0x08004000, LENGTH = 48K    /* Application */
  CONFIG (r)      : ORIGIN = 0x08010000, LENGTH = 4K     /* Checksum storage */
  RAM (rwx)       : ORIGIN = 0x20000000, LENGTH = 64K    /* Shared RAM */
}

SECTIONS
{
  /* Bootloader Section */
  .bootloader :
  {
    KEEP(*(.bootloader_vector_table))  /* Vector table for bootloader */
    *(.bootloader*)                     /* Bootloader code */
    . = ALIGN(4);
    _ebootloader = .;                   /* End of bootloader */
  } > BOOTLOADER

  /* Application Section */
  .app :
  {
    _sapp = .;                         /* Start of application */
    KEEP(*(.app_vector_table))         /* Application vector table */
    *(.app*)                           /* Application code */
    . = ALIGN(4);
    _eapp = .;                         /* End of application */
  } > APP

  /* Checksum Storage in CONFIG */
  .config :
  {
    _checksum = .;                     /* Checksum symbol */
    . += 4;                            /* Reserve 4 bytes */
  } > CONFIG

  /* Standard sections (.data, .bss, .heap, .stack) */
  /* ... (configure as needed) ... */
}
