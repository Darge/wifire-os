#include <asm.h>
#include <mips/m32c0.h>

        .file "thread.c" // to chyba nn

        .global saved_context
        .global ctx_saved
        .data
ctx_saved: .space 1000

        .set	noreorder       // Don't allow the assembler to reorder instructions.

        .local tlb_refill
        .local cache_error
        .local general_exception
        .local irq_handler
        .local irq0
        .local irq1
        .local irq2
        .local irq3
        .local irq4
        .local irq5
        .local irq6
        .local irq7

        .section .ebase

        .org 0x0

tlb_refill:
        mfc0 $k0, C0_CONTEXT        # Load UPT address which contains PTE
        sra $k0,1                   # PT should be located in KSEG2
        lw  $k1, 0($k0)
        mtc0 $k1, C0_ENTRYLO0
        lw  $k1, 4($k0)
        mtc0 $k1, C0_ENTRYLO1
        ehb                         # mtc0, Hazard on tlbwi
        tlbwr
        eret

        .org 0x100
cache_error:
1:      j       1b
        nop

        .org 0x180
general_exception:
        mfc0    $k0, C0_CAUSE
        andi    $k0, $k0, CR_X_MASK
        srl     $k0, $k0, CR_X_SHIFT
        li      $k1, EXC_SYS
        beq     $k1, $k0, 2f
        nop

        la      $k1, general_exception_table
        sll     $k0, $k0, 2   /* Multiply exc_code by sizeof(void *) */  
        add     $k1, $k1, $k0 /* Add exc_code */  
        lw      $k1, 0($k1)   /* Load the address of a handler */
        beqz    $k1, 1f       /* If NULL then call kernel_oops */
        nop
        jr      $k1           /* Jump to handler */ 
        nop

1:      jal     kernel_oops
        nop

        /* syscalls not implemented yet */
2:      j       2b
        nop

        .org 0x200
irq0:
1:      j       1b
        nop

        .org 0x220
irq1:
1:      j       1b
        nop

        .org 0x240
irq2:
1:      j       1b
        nop

        .org 0x260
irq3:
1:      j       1b
        nop

        .org 0x280
irq4:
1:      j       1b
        nop

        .org 0x2a0
irq5:
1:      j       1b
        nop

        .org 0x2c0
irq6:
1:      j       1b
        nop

        .org 0x2e0
irq7:
        la      $k1,hardclock
        j       irq_handler
        nop

        .org 0x300

#define REG_STACK_STORE_SIZE 76

        //.set	noat            // Don't allow the assembler to use r1(at) for synthetic instr.

irq_handler:
        /* Please, no nested interrupts for now. */
        di

        /* Allocate space for register storage on the stack. */
        subu    $sp, $sp, REG_STACK_STORE_SIZE

        /*
         * Save registers on the stack.
         * Saving all t, a and v registers, as well as LO and HI.
         * k registers do not require saving.
         * s and sp registers are saved by compiler-generated
         * procedure entry, if the procedure modifies them.
         */
        la      $at, ctx_saved
        sw      $t0, ($at)
        sw      $t1, 4($at)
        sw      $t2, 8($at)
        sw      $t3, 12($at)
        sw      $t4, 16($at)
        sw      $t5, 20($at)
        sw      $t6, 24($at)
        sw      $t7, 28($at)
        sw      $t8, 32($at)
        sw      $t9, 36($at)
        sw      $a0, 40($at)
        sw      $a1, 44($at)
        sw      $a2, 48($at)
        sw      $a3, 52($at)
        sw      $v0, 56($at)
        sw      $v1, 60($at)
        sw      $ra, 64($at)

        mflo    $k0
        sw      $k0, 68($at)
        mfhi    $k0
        sw      $k0, 72($at)

        /* Call the C routine. */
        jalr    $k1
        nop

        /* Restore registers. */
        lw      $k0, 72($at)
        mthi    $k0
        lw      $k0, 68($at)
        mtlo    $k0

        lw      $ra, 64($at)
        lw      $v1, 60($at)
        lw      $v0, 56($at)
        lw      $a3, 52($at)
        lw      $a2, 48($at)
        lw      $a1, 44($at)
        lw      $a0, 40($at)
        lw      $t9, 36($at)
        lw      $t8, 32($at)
        lw      $t7, 28($at)
        lw      $t6, 24($at)
        lw      $t5, 20($at)
        lw      $t4, 16($at)
        lw      $t3, 12($at)
        lw      $t2, 8($at)
        lw      $t1, 4($at)
        lw      $t0, ($at)

        /* Free stack frame */
        addu    $sp, $sp, REG_STACK_STORE_SIZE

        /* Re-enable interrupts. */
        ei

        eret
