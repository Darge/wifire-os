#include <libkern.h>
#include <context.h>

//extern void irq7();
extern const char _ebase[];
void ctx_init(ctx_t *ctx, void (*target)(), void *sp, bool falseStack) {
  register void *gp asm("$gp");

  bzero(ctx, sizeof(ctx_t));

  #define ZEROOOOO 15

  ctx->t0 = ZEROOOOO+1;
  ctx->t1 = ZEROOOOO;
  ctx->t2 = ZEROOOOO;
  ctx->t3 = ZEROOOOO;
  ctx->t4 = ZEROOOOO;
  ctx->t5 = ZEROOOOO;
  ctx->t6 = ZEROOOOO;
  ctx->t7 = ZEROOOOO;
  ctx->t8 = ZEROOOOO;
  ctx->t9 = ZEROOOOO;
  ctx->a0 = ZEROOOOO; // 10
  ctx->a1 = ZEROOOOO;
  ctx->a2 = ZEROOOOO;
  ctx->a3 = ZEROOOOO;
  ctx->v0 = ZEROOOOO;
  ctx->v1 = ZEROOOOO; // 15
  //ctx->ra = (intptr_t) (((char*)(_ebase)) + 0x2e0); // 16
  
  ctx->ra = (intptr_t)0x801003c8;
  ctx->hi = ZEROOOOO;
  ctx->lo = ZEROOOOO;
  ctx->epc = (intptr_t)target; /* How to do it with the label? */
  ctx->gp = (intptr_t)gp; // 20
  ctx->fp = ZEROOOOO; // 21
  ctx->s0 = ZEROOOOO;
  ctx->s1 = ZEROOOOO;
  ctx->s2 = ZEROOOOO;
  ctx->s3 = ZEROOOOO; // 25
  ctx->s4 = ZEROOOOO;
  ctx->s5 = ZEROOOOO;
  ctx->s6 = ZEROOOOO;
  ctx->s7 = ZEROOOOO;


  if (falseStack) {
    log("sp: %p", sp);
    ctx->sp = (intptr_t)((char*)sp - 128); // change to sizeof(ctx_t)
    *((ctx_t*)ctx->sp) = *ctx;
    log("ctx->sp: %p", (void*)ctx->sp);
  }
  else {
    ctx->sp = (intptr_t)sp;
  }
}

void ctx_switch(ctx_t *from, ctx_t *to) {
  if (!ctx_save(from))
    ctx_load(to);
}

void ctx_switch_interrupt(ctx_t *from, ctx_t *to) {
  if (!ctx_save(from))
    ctx_load_interrupt(to);
}
