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
    cld

    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    mov %rsp, %rdi
    call isr_handler

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax

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
ISR 32
ISR 33
ISR 34
ISR 35
ISR 36
ISR 37
ISR 38
ISR 39
ISR 40
ISR 41
ISR 42
ISR 43
ISR 44
ISR 45
ISR 46
ISR 47
ISR 48
ISR 49

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
    .quad isr_stub_32
    .quad isr_stub_33
    .quad isr_stub_34
    .quad isr_stub_35
    .quad isr_stub_36
    .quad isr_stub_37
    .quad isr_stub_38
    .quad isr_stub_39
    .quad isr_stub_40
    .quad isr_stub_41
    .quad isr_stub_42
    .quad isr_stub_43
    .quad isr_stub_44
    .quad isr_stub_45
    .quad isr_stub_46
    .quad isr_stub_47
    .quad isr_stub_48
    .quad isr_stub_49
