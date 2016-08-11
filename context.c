#include <libkern.h>
#include <context.h>

//extern void irq7();

void ctx_init(ctx_t *ctx, void (*target)(), void *sp) {
  register void *gp asm("$gp");

  bzero(ctx, sizeof(ctx_t));

  ctx->t0 = 0;
  ctx->t1 = 0;
  ctx->t2 = 0;
  ctx->t3 = 0;
  ctx->t4 = 0;
  ctx->t5 = 0;
  ctx->t6 = 0;
  ctx->t7 = 0;
  ctx->t8 = 0;
  ctx->t9 = 0;
  ctx->a0 = 0; // 10
  ctx->a1 = 0;
  ctx->a2 = 0;
  ctx->a3 = 0;
  ctx->v0 = 0;
  ctx->v1 = 0; // 15
  ctx->ra = (intptr_t)0x2e0; // 16
  ctx->hi = 0;
  ctx->lo = 0;
  ctx->epc = (intptr_t)target; /* How to do it with the label? */
  ctx->gp = (intptr_t)gp; // 20
  ctx->fp = 0; // 21
  ctx->s0 = 0;
  ctx->s1 = 0;
  ctx->s2 = 0;
  ctx->s3 = 0; // 25
  ctx->s4 = 0;
  ctx->s5 = 0;
  ctx->s6 = 0;
  ctx->s7 = 0;



  ctx->sp = (intptr_t)((char*)sp + 128); // 30 // change to sizeof(ctx_t)
  *((ctx_t*)sp) = *ctx;
}

void ctx_switch(ctx_t *from, ctx_t *to) {
  if (!ctx_save(from))
    ctx_load(to);
}

void ctx_switch_interrupt(ctx_t *from, ctx_t *to) {
  if (!ctx_save(from))
    ctx_load_interrupt(to);
}
