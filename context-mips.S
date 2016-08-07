#include "asm.h"

    .set noreorder

/* uint32_t ctx_save(ctx_t *ctx) */
LEAF(ctx_save)

    sw $ra, ( 16*4)($a0)
    sw $fp, ( 21*4)($a0)
    sw $sp, ( 30*4)($a0)
    sw $gp, ( 20*4)($a0)
    sw $s0, ( 22*4)($a0)
    sw $s1, ( 23*4)($a0)
    sw $s2, ( 24*4)($a0)
    sw $s3, ( 25*4)($a0)
    sw $s4, ( 26*4)($a0)
    sw $s5, ( 27*4)($a0)
    sw $s6, (28*4)($a0)
    sw $s7, (29*4)($a0)

    /* Return 0 */
    jr $ra
    move $v0, $zero

END(ctx_save)

/* void ctx_load(const ctx_t *ctx) */
LEAF(ctx_load)

    lw $ra, ( 16*4)($a0)
    lw $fp, ( 21*4)($a0)
    lw $sp, ( 30*4)($a0)
    lw $gp, ( 20*4)($a0)
    lw $s0, ( 22*4)($a0)
    lw $s1, ( 23*4)($a0)
    lw $s2, ( 24*4)($a0)
    lw $s3, ( 25*4)($a0)
    lw $s4, ( 26*4)($a0)
    lw $s5, ( 27*4)($a0)
    lw $s6, (28*4)($a0)
    lw $s7, (29*4)($a0)

    /*
     * Note that the following will not return from ctx_load, because
     * $ra has been overwritten with the value previously stored by
     * ctx_store. Instead, it resumes the context, and returns 1
     * there.
     */
    jr $ra
    li $v0, 1

END(ctx_load)

/* void ctx_call(const ctx_t *ctx, void *fn) */
LEAF(ctx_call)

    lw $ra, ( 16*4)($a0)
    lw $fp, ( 21*4)($a0)
    lw $sp, ( 30*4)($a0)
    lw $gp, ( 20*4)($a0)
    lw $s0, ( 22*4)($a0)
    lw $s1, ( 23*4)($a0)
    lw $s2, ( 24*4)($a0)
    lw $s3, ( 25*4)($a0)
    lw $s4, ( 26*4)($a0)
    lw $s5, ( 27*4)($a0)
    lw $s6, (28*4)($a0)
    lw $s7, (29*4)($a0)

    jr $a1
    li $v0, 1

END(ctx_call)
