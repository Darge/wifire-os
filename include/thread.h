#ifndef __THREAD_H__
#define __THREAD_H__

#include "common.h"
#include "queue.h"
#include "context.h"

#define SUCCESS 1
#define FAILURE 0

typedef uint8_t prio_t;

typedef struct td_sched td_sched_t;

typedef struct stack {
  uint8_t *stk_base; /* stack base */
  size_t stk_size;   /* stack length */
} stack_t;

typedef struct thread {
  TAILQ_ENTRY(thread) td_runq;    /* a link on run queue */
  TAILQ_ENTRY(thread) td_sleepq;  /* a link on sleep queue */
  td_sched_t *td_sched;           /* scheduler-specific data */
  prio_t td_priority;
  stack_t td_stack;
  ctx_t td_context;
  enum {
    TDS_INACTIVE = 0x0,
    TDS_WAITING,
    TDS_READY,
    TDS_RUNNING
  } td_state;
} thread_t;

int thread_create(thread_t* thread_ptr, prio_t priority, void (*function_ptr)(void * ) , void *args);
void thread_exit() __attribute__((noreturn));
void thread_destroy(thread_t *);
int thread_join(thread_t *);

#endif // __THREAD_H__
