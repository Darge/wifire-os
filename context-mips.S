#include "asm.h"

    .set noreorder
    .set noat

/* uint32_t ctx_save(ctx_t *ctx) */
LEAF(ctx_save)

    sw $ra, ( 0*4)($a0)
    sw $fp, ( 1*4)($a0)
    sw $sp, ( 2*4)($a0)
    sw $gp, ( 3*4)($a0)
    sw $s0, ( 4*4)($a0)
    sw $s1, ( 5*4)($a0)
    sw $s2, ( 6*4)($a0)
    sw $s3, ( 7*4)($a0)
    sw $s4, ( 8*4)($a0)
    sw $s5, ( 9*4)($a0)
    sw $s6, (10*4)($a0)
    sw $s7, (11*4)($a0)

    /* Return 0 */
    jr $ra
    move $v0, $zero

END(ctx_save)

/* void ctx_load(const ctx_t *ctx) */
LEAF(ctx_load)

    lw $ra, ( 0*4)($a0)
    lw $fp, ( 1*4)($a0)
    lw $sp, ( 2*4)($a0)
    lw $gp, ( 3*4)($a0)
    lw $s0, ( 4*4)($a0)
    lw $s1, ( 5*4)($a0)
    lw $s2, ( 6*4)($a0)
    lw $s3, ( 7*4)($a0)
    lw $s4, ( 8*4)($a0)
    lw $s5, ( 9*4)($a0)
    lw $s6, (10*4)($a0)
    lw $s7, (11*4)($a0)

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

    lw $ra, ( 0*4)($a0)
    lw $fp, ( 1*4)($a0)
    lw $sp, ( 2*4)($a0)
    lw $gp, ( 3*4)($a0)
    lw $s0, ( 4*4)($a0)
    lw $s1, ( 5*4)($a0)
    lw $s2, ( 6*4)($a0)
    lw $s3, ( 7*4)($a0)
    lw $s4, ( 8*4)($a0)
    lw $s5, ( 9*4)($a0)
    lw $s6, (10*4)($a0)
    lw $s7, (11*4)($a0)

    jr $a1
    li $v0, 1

END(ctx_call)




/* uint32_t ctx_save_interrupt(ctx_t *ctx) */
LEAF(ctx_save_interrupt)
# Move stuff from place_for_ctx to ctx
# store something on the stack first, so that we have a free register

    subu    $sp, $sp, 8 # not sure, so just to be safe
    sw      $t0, 4($sp)
# now we can use t0 as a temporary

    la $at, (place_for_ctx)

    # sw $ra, ( 0*4)($a0)
    lw $t0, 64($at)
    sw $t0, (0*4)($a0)
    # sw $fp, ( 1*4)($a0)
    lw $t0, 112($at)
    sw $t0, (1*4)($a0)
    # sw $sp, ( 2*4)($a0)
    lw $t0, 108($at)
    sw $t0, (2*4)($a0)
    # sw $gp, ( 3*4)($a0)
    lw $t0, 116($at) 
    sw $t0, (3*4)($a0)
    # sw $s0, ( 4*4)($a0)
    lw $t0, 76($at)
    sw $t0, (4*4)($a0)
    # sw $s1, ( 5*4)($a0)
    lw $t0, 80($at)
    sw $t0, (5*4)($a0)
    # sw $s2, ( 6*4)($a0)
    lw $t0, 84($at)
    sw $t0, (6*4)($a0)
    # sw $s3, ( 7*4)($a0)
    lw $t0, 88($at)
    sw $t0, (7*4)($a0)
    # sw $s4, ( 8*4)($a0)
    lw $t0, 92($at)
    sw $t0, (8*4)($a0)
    # sw $s5, ( 9*4)($a0)
    lw $t0, 96($at)
    sw $t0, (9*4)($a0)
    # sw $s6, (10*4)($a0)
    lw $t0, 100($at)
    sw $t0, (10*4)($a0)
    # sw $s7, (11*4)($a0)
    lw $t0, 104($at)
    sw $t0, (11*4)($a0)


    lw $t0, 4($sp)
    addu    $sp, $sp, 8

    /* Return 0 */
    jr $ra
    move $v0, $zero

END(ctx_save_interrupt)

/* void ctx_load_interrupt(const ctx_t *ctx) */
LEAF(ctx_load_interrupt)
# move stuff from ctx to place_for_ctx
    la $at, (place_for_ctx)

    subu    $sp, $sp, 8 # not sure, so just to be safe
    sw      $t0, 4($sp)
# now we can use t0 as a temporary

    # sw $ra, ( 0*4)($a0)
    lw $t0, (0*4)($a0)
    sw $t0, 64($at)
    # sw $fp, ( 1*4)($a0)
    lw $t0, (1*4)($a0)
    sw $t0, 112($at)
    # sw $sp, ( 2*4)($a0)
    lw $t0, (2*4)($a0)
    sw $t0, 108($at)
    # sw $gp, ( 3*4)($a0)
    lw $t0, (3*4)($a0)
    sw $t0, 116($at) 
    # sw $s0, ( 4*4)($a0)
    lw $t0, (4*4)($a0)
    sw $t0, 76($at)
    # sw $s1, ( 5*4)($a0)
    lw $t0, (5*4)($a0)
    sw $t0, 80($at)
    # sw $s2, ( 6*4)($a0)
    lw $t0, (6*4)($a0)
    sw $t0, 84($at)
    # sw $s3, ( 7*4)($a0)
    lw $t0, (7*4)($a0)
    sw $t0, 88($at)
    # sw $s4, ( 8*4)($a0)
    lw $t0, (8*4)($a0)
    sw $t0, 92($at)
    # sw $s5, ( 9*4)($a0)
    lw $t0, (9*4)($a0)
    sw $t0, 96($at)
    # sw $s6, (10*4)($a0)
    lw $t0, (10*4)($a0)
    sw $t0, 100($at)
    # sw $s7, (11*4)($a0)
    lw $t0, (11*4)($a0)
    sw $t0, 104($at)

    lw $t0, 4($sp)
    addu    $sp, $sp, 8




    /*
     * Note that the following will not return from ctx_load, because
     * $ra has been overwritten with the value previously stored by
     * ctx_store. Instead, it resumes the context, and returns 1
     * there.
     */
    jr $ra
    li $v0, 1

END(ctx_load_interrupt)