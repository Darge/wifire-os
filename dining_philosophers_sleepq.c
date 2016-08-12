#include <libkern.h>
#include <mutex_sleepq.h>
#include <thread.h>
#include <clock.h>
#include <sched.h>
#include <rtc.h>
#include <critical_section.h>


#ifdef _KERNELSPACE

static mtx_sleepq_t mtxs[5];

static int left_mtx[] = {0, 1, 2, 3, 4};
static int right_mtx[] = {4, 0, 1, 2, 3};

static int counter[] = {0, 0, 0, 0, 0};

#define PRINT_MUTXS cs_enter(); \
    log("Thread: %s, Mutexes states: %d %d %d %d %d",thread_self()->td_name, (int)mtxs[0], (int)mtxs[1], (int)mtxs[2], (int)mtxs[3], (int)mtxs[4]); \
    cs_leave();

static void philosopher_loop(int number) {
    while (true) {
      int first_mtx = min(left_mtx[number], right_mtx[number]);
      int second_mtx = max(left_mtx[number], right_mtx[number]);
      PRINT_MUTXS;


      mdelay(2);
      mtx_sleepq_lock(&mtxs[first_mtx]);
      PRINT_MUTXS;
      mtx_sleepq_lock(&mtxs[second_mtx]);
      PRINT_MUTXS;

      counter[number]++;
      log("Number %d acquired two locks: %d, %d. Counters: %d %d %d %d %d", number, first_mtx, second_mtx, counter[0], counter[1], counter[2], counter[3], counter[4]);
      mdelay(1);
      //log("Number %d releasing two locks: %d, %d. Counters: %d %d %d %d %d", number, first_mtx, second_mtx, counter[0], counter[1], counter[2], counter[3], counter[4]);

      mtx_sleepq_unlock(&mtxs[first_mtx]);
      mtx_sleepq_unlock(&mtxs[second_mtx]);
    }
}

static void demo_thread_0() {
  int number = 0;
  philosopher_loop(number);
}

static void demo_thread_1() {
  int number = 1;
  philosopher_loop(number);
}

static void demo_thread_2() {
  int number = 2;
  philosopher_loop(number);
}

static void demo_thread_3() {
  int number = 3;
  philosopher_loop(number);
}

static void demo_thread_4() {
  int number = 4;
  philosopher_loop(number);
}


int main() {
  sched_init();

  mtx_sleepq_init(&mtxs[0]);
  mtx_sleepq_init(&mtxs[1]);
  mtx_sleepq_init(&mtxs[2]);
  mtx_sleepq_init(&mtxs[3]);
  mtx_sleepq_init(&mtxs[4]);

  thread_t *t0 = thread_create("t0", demo_thread_0);
  thread_t *t1 = thread_create("t1", demo_thread_1);
  thread_t *t2 = thread_create("t2", demo_thread_2);
  thread_t *t3 = thread_create("t3", demo_thread_3);
  thread_t *t4 = thread_create("t4", demo_thread_4);

  sched_add(t0);
  sched_add(t1);
  sched_add(t2);
  sched_add(t3);
  sched_add(t4);


  sched_run(1);
}

#endif // _KERNELSPACE
