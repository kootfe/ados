.section .multiboot2
.align 8

header_start:
    .long 0xE85250D6
    .long 0
    .long header_end - header_start
    .long -(0xE85250D6 + 0 + (header_end - header_start))

    .align 8
    .short 0
    .long 8
header_end:


/* ---------------- STACK ---------------- */
.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:


/* ---------------- PAGE TABLES (MUST BE .BSS OR .DATA, BUT ZEROED ONLY IF .BSS) ---------------- */
.section .bss
.align 4096

pml4:
    .skip 4096

pdpt:
    .skip 4096

pd:
    .skip 4096


/* ---------------- GDT ---------------- */
.section .rodata
.align 16

gdt_start:
    .quad 0x0000000000000000        /* null */
    .quad 0x00AF9A000000FFFF        /* code */
    .quad 0x00AF92000000FFFF        /* data */
gdt_end:

gdt_desc:
    .word gdt_end - gdt_start - 1
    .quad gdt_start


/* ---------------- TEXT ---------------- */
.section .text
.code32
.globl _start
.extern kmain


_start:
    cli

    /* stack */
    mov $stack_top, %esp

    /* zero page tables */
    xor %eax, %eax
    cld

    mov $pml4, %edi
    mov $4096/4, %ecx
    rep stosl

    mov $pdpt, %edi
    mov $4096/4, %ecx
    rep stosl

    mov $pd, %edi
    mov $4096/4, %ecx
    rep stosl


    /* ---------------- BUILD PAGING ---------------- */

    /* pml4[0] = pdpt | flags */
    mov $pdpt, %eax
    or $0x03, %eax
    mov %eax, pml4
    movl $0, pml4+4

    /* pdpt[0] = pd | flags */
    mov $pd, %eax
    or $0x03, %eax
    mov %eax, pdpt
    movl $0, pdpt+4


    /* identity map first 1GB (512 entries * 2MB) */
    mov $pd, %edi
    xor %ebx, %ebx
    mov $512, %ecx

1:
    mov %ebx, %eax
    or $0x83, %eax          /* present + write + huge page */
    mov %eax, (%edi)
    movl $0, 4(%edi)

    add $8, %edi
    add $0x200000, %ebx
    loop 1b


    /* load page tables */
    mov $pml4, %eax
    mov %eax, %cr3


    /* enable PAE */
    mov %cr4, %eax
    or $0x20, %eax
    mov %eax, %cr4


    /* enable long mode */
    mov $0xC0000080, %ecx
    rdmsr
    or $0x00000100, %eax
    wrmsr


    /* enable paging */
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0


    /* load GDT */
    lgdt gdt_desc

    ljmp $0x08, $long_mode_start


/**************** LONG MODE ****************/
.code64
long_mode_start:
    sti
    lea stack_top(%rip), %rsp
    and $-16, %rsp

    cld
    call kmain

.hang:
    cli
    hlt
    jmp .hang
