#include <libkern.h>
#include <mutex_yield.h>
#include <thread.h>
#include <clock.h>
#include <sched.h>


#ifdef _KERNELSPACE

static mtx_yield_t mtxs[4];
//         P0
//     f3       f0
//  P3             P1
//     f2       f1
//         P2
//
static int left_mtx[] = {0, 1, 2, 3};
static int right_mtx[] = {3, 2, 1, 0};

static void demo_thread_1() {
  int number = 0;
  while (true) {
    int first_mtx = min(left_mtx[number], right_mtx[number]);
    int second_mtx = max(left_mtx[number], right_mtx[number]);

    mtx_yield_lock(&mtxs[first_mtx]);
    mtx_yield_lock(&mtxs[second_mtx]);
    log("Number %d acquired two locks: %d, %d.", number, first_mtx, second_mtx);
    mtx_yield_unlock(&mtxs[first_mtx]);
    mtx_yield_unlock(&mtxs[second_mtx]);
  }
}

static void demo_thread_2() {
  int number = 1;
  while (true) {
    int first_mtx = min(left_mtx[number], right_mtx[number]);
    int second_mtx = max(left_mtx[number], right_mtx[number]);

    mtx_yield_lock(&mtxs[first_mtx]);
    mtx_yield_lock(&mtxs[second_mtx]);
    log("Number %d acquired two locks: %d, %d.", number, first_mtx, second_mtx);
    mtx_yield_unlock(&mtxs[first_mtx]);
    mtx_yield_unlock(&mtxs[second_mtx]);
  }
}

static void demo_thread_3() {
  int number = 2;
  while (true) {
    int first_mtx = min(left_mtx[number], right_mtx[number]);
    int second_mtx = max(left_mtx[number], right_mtx[number]);

    mtx_yield_lock(&mtxs[first_mtx]);
    mtx_yield_lock(&mtxs[second_mtx]);
    log("Number %d acquired two locks: %d, %d.", number, first_mtx, second_mtx);
    mtx_yield_unlock(&mtxs[first_mtx]);
    mtx_yield_unlock(&mtxs[second_mtx]);
  }
}

static void demo_thread_4() {
  int number = 3;
  while (true) {
    int first_mtx = min(left_mtx[number], right_mtx[number]);
    int second_mtx = max(left_mtx[number], right_mtx[number]);

    mtx_yield_lock(&mtxs[first_mtx]);
    mtx_yield_lock(&mtxs[second_mtx]);
    log("Number %d acquired two locks: %d, %d.", number, first_mtx, second_mtx);
    mtx_yield_unlock(&mtxs[first_mtx]);
    mtx_yield_unlock(&mtxs[second_mtx]);
  }
}

int main() {
  sched_init();

  mtx_yield_init(&mtxs[0]);
  mtx_yield_init(&mtxs[1]);
  mtx_yield_init(&mtxs[2]);
  mtx_yield_init(&mtxs[3]);

  thread_t *t1 = thread_create("t1", demo_thread_1);
  thread_t *t2 = thread_create("t2", demo_thread_2);
  thread_t *t3 = thread_create("t3", demo_thread_3);
  thread_t *t4 = thread_create("t4", demo_thread_4);

  sched_add(t1);
  sched_add(t2);
  sched_add(t3);
  sched_add(t4);

  sched_run(100);
}

#endif // _KERNELSPACE
