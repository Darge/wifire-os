#ifndef __MUTEX_SLEEPQ_H__
#define __MUTEX_SLEEPQ_H__

#include <common.h>
#include <mips.h>

typedef volatile uintptr_t mtx_yield_t;

void mtx_sleepq_init(mtx_yield_t *mtx);
void mtx_sleepq_lock(mtx_yield_t *mtx);
void mtx_sleepq_unlock(mtx_yield_t *mtx);

#endif /* __MUTEX_SLEEPQ_H__ */
