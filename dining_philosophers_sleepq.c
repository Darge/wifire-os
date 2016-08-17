#include <libkern.h>
#include <mutex_sleepq.h>
#include <thread.h>
#include <clock.h>
#include <sched.h>
#include <rtc.h>


#ifdef _KERNELSPACE

static mtx_sleepq_t mtxs[4];
//         P0
//     f3       f0
//  P3             P1
//     f2       f1
//         P2
//
static int left_mtx[] = {0, 1, 2, 3};
static int right_mtx[] = {3, 0, 1, 2};
static int counter[] = {0, 0, 0, 0};

// static void mdelay (unsigned msec) {
//   unsigned now = clock_get_ms();
//   unsigned final = now + msec;
//   while (final > clock_get_ms());
// }


static void philosopher_loop(int number) {
    while (true) {
      int first_mtx = min(left_mtx[number], right_mtx[number]);
      int second_mtx = max(left_mtx[number], right_mtx[number]);



      //log("Number %d will try to lock something", number);
      mdelay(3);
      mtx_sleepq_lock(&mtxs[first_mtx]);
      mtx_sleepq_lock(&mtxs[second_mtx]);

      counter[number]++;
      log("Number %d acquired two locks: %d, %d. Counters: %d %d %d %d", number, first_mtx, second_mtx, counter[0], counter[1], counter[2], counter[3]);
      mdelay(3);

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

int main() {
  sched_init();

  mtx_sleepq_init(&mtxs[0]);
  mtx_sleepq_init(&mtxs[1]);
  mtx_sleepq_init(&mtxs[2]);
  mtx_sleepq_init(&mtxs[3]);

  thread_t *t1 = thread_create("t0", demo_thread_0);
  thread_t *t2 = thread_create("t1", demo_thread_1);
  thread_t *t3 = thread_create("t2", demo_thread_2);
  thread_t *t4 = thread_create("t3", demo_thread_3);

  sched_add(t1);
  sched_add(t2);
  sched_add(t3);
  sched_add(t4);


  sched_run(10);
}

#endif // _KERNELSPACE
