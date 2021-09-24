OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
    . = 0x100000;
    .text :
    {
        code = .;
        *(.multiboot)
        *(.text)
        . = ALIGN(4K);
    }
    .data :
    {
        data = .;
        *(.data)
        *(.rodata)
        . = ALIGN(4K);
    }
    .bss :
    {
        bss = .;
        *(.bss)
        . = ALIGN(4K);
    }
    end =.;
}
