#include <libkern.h>
#include <context.h>

void ctx_init(ctx_t *ctx, void (*target)(), void *sp, bool prepare_stack) {
  register void *gp asm("$gp");

  bzero(ctx, sizeof(ctx_t));

  /* How to do it with the label? Here, I took the address of
     label new_thread (from intr.S) manually, from gdb. */
  ctx->ra = (intptr_t)0x801003b0;
  ctx->epc = (intptr_t)target;
  ctx->gp = (intptr_t)gp;


  if (prepare_stack) {
    ctx->sp = (intptr_t)((char *)sp - 128); // change to sizeof(ctx_t)
    *((ctx_t *)ctx->sp) = *ctx;
  } else {
    ctx->sp = (intptr_t)sp;
  }
}

void ctx_switch(ctx_t *from, ctx_t *to) {
  if (!ctx_save(from))
    ctx_load(to);
}
