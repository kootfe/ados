ENTRY(_start)

SECTIONS
{
        . = 1M;

        .multiboot2 : {
            *(.multiboot2)
        }

        .text : {
            *(.text*)
        }

        .data : {
            *(.data*)
        }

        .bss : {
            __bss_start = .;
            *(.bss*)
            *(COMMON)
            __bss_end = .;
        }

        .rodata : {
            *(.rodata*)
        }
}
