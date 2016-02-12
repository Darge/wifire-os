#ifndef __RUN_QUEUE_H__
#define __RUN_QUEUE_H__

typedef struct runq_t {
  uint8_t *stk_base; /* stack base */
  size_t stk_size;   /* stack length */
} runq_t;


void runq_init(runq_t *);
void runq_add(runq_t *, thread_t *);
thread_t *runq_choose(runq_t *);
void runq_remove(runq_t *, thread_t *);

#endif // __RUN_QUEUE_H__
