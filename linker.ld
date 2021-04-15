OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
    . = 1M;
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }
    .data : ALIGN(4K)
    {
        *(.data)
    }
    .bss  : ALIGN(4K)
    {
        *(.bss)
    }
}
