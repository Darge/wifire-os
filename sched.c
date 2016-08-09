#include <libkern.h>
#include <sched.h>
#include <runq.h>
#include "context.h"
#include <thread.h>
#include <callout.h>
#include <interrupts.h>

static runq_t runq;
static callout_t callout[2000];
static int current_callout = 0;

void sched_init() {
  runq_init(&runq);
}

void sched_preempt() {
  log("Preempting.");
   callout_setup(&callout[current_callout+1], 5, sched_preempt, NULL);
  current_callout = current_callout+1;
  sched_switch();
}

static thread_t* sched_choose() {
  thread_t* td = runq_choose(&runq);
  if (td)
    runq_remove(&runq, td);
  return td;
}

void sched_run() {
  log("Scheduler is run.");
  thread_t* new_td = sched_choose();

  if (!new_td)
    panic("There are no threads to be executed\n");

  current_callout = 0;
  callout_setup(&callout[current_callout], 5, sched_preempt, NULL); /* Bad stuff here. */

  thread_switch_to(new_td);
}

void sched_add(thread_t *td) {
  log("Adding a thread to the runqueue.");
  runq_add(&runq, td);
}

// void sched_interrupt() {
//   log("A thread has been interrupted.");
//   thread_t* current_td = td_running;
//   thread_t* new_td = sched_choose();

//   if (!new_td)
//     //return;
//     panic("new_td is NULL");

//   sched_add(current_td);
//   thread_switch_to_interrupt(new_td);
// }

void sched_switch() {
  log("A thread has yielded.");
  thread_t* current_td = td_running;
  thread_t* new_td = sched_choose();

  if (!new_td)
    //return;
    panic("new_td is NULL");

  sched_add(current_td);
  thread_switch_to(new_td);
}


#ifdef _KERNELSPACE

static void demo_thread_1() {
  log("entering demo_thread_1");
  //intr_enable();
  while (true) {
    kprintf("demo_thread_1 running.\n");
    for (int i = 0; i < 20000; i++) {};
    //sched_yield();
  }
}

static void demo_thread_2() {
  log("entering demo_thread_2");
  //__extension__ ({ asm("lw $k0, demo_thread_"); });
  //__extension__ ({ asm("mtc0 $k0,$14"); });
  intr_enable();
  // __extension__ ({ asm("eret"); });
  while (true) {
    kprintf("demo_thread_2 running\n");
    for (int i = 0; i < 20000; i++) {};
    //sched_yield();
  }
}

// static void demo_thread_3() {
//   intr_enable();
//   while (true) {
//     kprintf("demo_thread_3 running\n");
//     for (int i = 0; i < 100000; i++) {};
//     //sched_yield();
//   }
// }

int main() {
  sched_init();

  thread_t *td1 = thread_create(demo_thread_1);
  thread_t *td2 = thread_create(demo_thread_2);
  //thread_t *td3 = thread_create(demo_thread_3);

  sched_add(td1);
  sched_add(td2);
  // sched_add(td3);

  sched_run();

  panic("We should never get to this place.\n");
  return 0;
}

#endif // _KERNELSPACE
