#ifndef __RUN_QUEUE_H__
#define __RUN_QUEUE_H__

#include "queue.h"
#include "thread.h"


TAILQ_HEAD(runq_head, thread);

/*
  TODO: Comments
*/

typedef struct runq {
  struct runq_head runq_head;
  int stub;
} runq_t;



void runq_init(runq_t* runq_ptr);
void runq_add(runq_t* runq_ptr, struct thread* thread_ptr);
struct thread* runq_choose(runq_t* runq_ptr);
void runq_remove(runq_t* runq_ptr, struct thread* thread_ptr);

#endif // __RUN_QUEUE_H__
