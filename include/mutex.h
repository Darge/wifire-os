#ifndef __SYS_MUTEX_H__
#define __SYS_MUTEX_H__

#include <mutex.h>

extern volatile int cs_level;

void cs_enter();

void cs_leave();

#endif /* __SYS_MUTEX_H__ */
