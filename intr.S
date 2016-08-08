#include <asm.h>
#include <mips/m32c0.h>

        .set	noreorder       // Don't allow the assembler to reorder instructions.
        .data
//        .globl place_for_ctx
//        //.extern place_for_ctx, 200
//place_for_ctx: .word 0:30


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

        .set	noat            // Don't allow the assembler to use r1(at) for synthetic instr.

irq_handler:
        /* Please, no nested interrupts for now. */
        di

        /*
         * Save registers.
         * Saving all t, a and v registers, as well as LO and HI.
         * k registers do not require saving.
         * s and sp registers are saved by compiler-generated
         * procedure entry, if the procedure modifies them.
         */
        la      $at, (place_for_ctx)
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

        sw      $s0, 76($at)
        sw      $s1, 80($at)
        sw      $s2, 84($at)
        sw      $s3, 88($at)
        sw      $s4, 92($at)
        sw      $s5, 96($at)
        sw      $s6, 100($at)
        sw      $s7, 104($at)
        sw      $sp, 108($at)

        sw      $fp, 112($at)
        sw      $gp, 116($at)


        /* Call the C routine. */
        jalr    $k1
        nop

        /* Restore registers. */

        lw      $gp, 116($at)
        lw      $fp, 112($at)

        lw      $sp, 108($at)
        lw      $s7, 104($at)
        lw      $s6, 100($at)
        lw      $s5, 96($at)
        lw      $s4, 92($at)
        lw      $s3, 88($at)
        lw      $s2, 84($at)
        lw      $s1, 80($at)
        lw      $s0, 76($at)

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

        /* Re-enable interrupts. */
        ei

        eret
