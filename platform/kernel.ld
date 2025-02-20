MEMORY {
  dram_space (rwx) : ORIGIN = 0x80100000, LENGTH = 128
}

SECTIONS {
  .text : {
    kernel.o(.text.bios)
  } > dram_space
}