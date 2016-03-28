#include "common.h"
#include "run_queue.h"
#include "libkern.h"

void runq_init(runq_t* runq_ptr) {
  // nothing for now
  memset(runq_ptr, 0, sizeof *runq_ptr); /* What for? Is the size good? Make sure it is */
  TAILQ_INIT(&runq_ptr->runq_head);
}

void runq_add(runq_t* runq_ptr, struct thread* thread_ptr) { // TODO can I change it to thread_t?
  if (TAILQ_EMPTY(&runq_ptr->runq_head)) {
    TAILQ_INSERT_HEAD(&runq_ptr->runq_head, thread_ptr, td_runq);
    return;
  }

  bool inserted = false; // TODO delete
  struct thread* loop_td_ptr;

  TAILQ_FOREACH(loop_td_ptr, &runq_ptr->runq_head, td_runq) {
    if (loop_td_ptr->td_priority < thread_ptr->td_priority) {
      TAILQ_INSERT_BEFORE(loop_td_ptr, thread_ptr, td_runq);
      inserted = true;
      break; // TODO rework it to simply return faster
    }
  }

  if (!inserted) {
    TAILQ_INSERT_TAIL(&runq_ptr->runq_head, thread_ptr, td_runq);
  }
}

struct thread* runq_choose(runq_t* runq_ptr) {
  return TAILQ_FIRST(&runq_ptr->runq_head);
}

void runq_remove(runq_t* runq_ptr, struct thread* thread_ptr) {
  TAILQ_REMOVE(&runq_ptr->runq_head, thread_ptr, td_runq);
}