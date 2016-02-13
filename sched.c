#include <libkern.h>
#include "sched.h"
#include "run_queue.h"
#include "context.h"

/* Defines here or in sched.h? */
/* Should they be defines, or const ints? */
#define SCHEDULER_FREQUENCY 30 /* have to find a better way */
#define TIME_SLICE 10

static int sched_counter = 0;
static runq_t runq;

void sched_run() {
  if (++sched_counter < SCHEDULER_FREQUENCY)
    return;

  kprintf("Scheduler is run\n");
  sched_counter = 0;
}
/* What happens if there are no threads to run? */
thread_t* sched_choose() {
  return runq_choose(&runq);
}

void sched_add(thread_t *td) {
  runq_add(&runq, td);
}

void sched_switch(thread_t *oldtd, thread_t *newtd) {

}

void sched_prio(thread_t *td, prio_t prio) {
  td->td_priority = prio;
}
