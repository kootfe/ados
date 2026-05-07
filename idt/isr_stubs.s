.section .text
.code64

.globl isr_stub_table
.globl isr_common_stub
.extern isr_handler

# -- macros ---------------------------------------------------
.macro ISR n
.globl isr_stub_\n
isr_stub_\n:
    push $0
    push $\n
    jmp isr_common_stub
.endm

.macro ISR_ERR n
.globl isr_stub_\n
isr_stub_\n:
    push $\n
    jmp isr_common_stub
.endm

# -- common stub --------------------------------------------
isr_common_stub:
    push %rax
    push %rdi

    # Stack Layout:
    # [rsp+0] saved rax
    # [rsp+8] saved rdi
    # [rsp+16] vec number
    # [rsp+24] err code
    # [rsp+32] rip
    mov 16(%rsp), %rdi
    mov 24(%rsp), %rsi
    call isr_handler

    pop %rdi
    pop %rax
    add $16, %rsp
    iretq

# -- individual stubs ----------------------------------------
ISR 0 # Dvide by zero
ISR 1 # DB debuh
ISR 2 # NMI
ISR 3 # BreakPoint
ISR 4 # overflow
ISR 5 # bound range
ISR 6 # invalid opcode
ISR 7 # device not avalible
ISR_ERR 8 # double foult (error code 0)
ISR 9 # coprocesser overrun (legacy)
ISR_ERR 10 # invalid TSS
ISR_ERR 11 # segment not presen
ISR_ERR 12 # stack fault
ISR_ERR 13 # general protection
ISR_ERR 14 # page fault
ISR 15 # reserved
ISR 16 # x87 fpu
ISR_ERR 17
ISR 18
ISR 19
ISR 20
ISR_ERR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR_ERR 29
ISR_ERR 30
ISR 31

# -- stub pointer table ---------------------------------------
.type isr_stub_table, @object
isr_stub_table:
    .quad isr_stub_0
    .quad isr_stub_1
    .quad isr_stub_2
    .quad isr_stub_3
    .quad isr_stub_4
    .quad isr_stub_5
    .quad isr_stub_6
    .quad isr_stub_7
    .quad isr_stub_8
    .quad isr_stub_9
    .quad isr_stub_10
    .quad isr_stub_11
    .quad isr_stub_12
    .quad isr_stub_13
    .quad isr_stub_14
    .quad isr_stub_15
    .quad isr_stub_16
    .quad isr_stub_17
    .quad isr_stub_18
    .quad isr_stub_19
    .quad isr_stub_20
    .quad isr_stub_21
    .quad isr_stub_22
    .quad isr_stub_23
    .quad isr_stub_24
    .quad isr_stub_25
    .quad isr_stub_26
    .quad isr_stub_27
    .quad isr_stub_28
    .quad isr_stub_29
    .quad isr_stub_30
    .quad isr_stub_31
