#ifndef __SCHED_H__
#define __SCHED_H__

#include "thread.h"

/*
 * This is an extension to the thread structure
 * and is tailored to the requirements of this scheduler.
 */
typedef struct td_sched {
//    ts_timeslice;  /* As assigned based on priority. */ 
//    ts_remaining;  /* Remaining part of time slice. */
  int stub;
} td_sched_t;

/* Set the priority to an absolute value. */
void sched_prio(thread_t *td, prio_t prio);

/* Record the sleep time. */
void sched_sleep(thread_t *td);

/* Schedule a thread to resume execution and record how long it voluntarily slept. */
void sched_wakeup(thread_t *td);

/*
 * Switch threads. This function has to handle threads coming in
 * while blocked for some reason or running.
 */
void sched_switch(thread_t *oldtd, thread_t *newtd);

/*
 * Select the target thread queue and add a thread to it. 
 * Request preemption if required.
 */
void sched_add(thread_t *td);

/* Choose the highest priority thread to run. */
thread_t *sched_choose();

/* Runs main loop of the scheduler. */
void sched_run();

#endif // __SCHED_H__
