OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
    . = 1M;
    .text BLOCK(4K) : ALIGN(4K)
    {
        code = .;
        *(.multiboot)
        *(.text)
    }
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }
    .data BLOCK(4K) : ALIGN(4K)
    {
        data = .;
        *(.data)
    }
    .bss BLOCK(4K) : ALIGN(4K)
    {
        bss = .;
        *(.bss)
    }
    end =.;
}
