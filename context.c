#include <libkern.h>
#include <context.h>

//extern void irq7();
extern const char _ebase[];
void ctx_init(ctx_t *ctx, void (*target)(), void *sp, bool falseStack) {
  register void *gp asm("$gp");

  bzero(ctx, sizeof(ctx_t));

  #define ZEROOOOO 15

  /* How to do it with the label? Here, I took the address of
     label new_thread (from intr.S) manually, from gdb. */
  ctx->ra = (intptr_t)0x801003c8;
  ctx->epc = (intptr_t)target;
  ctx->gp = (intptr_t)gp;


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
