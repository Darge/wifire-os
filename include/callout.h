#ifndef __CALLOUT_H__
#define __CALLOUT_H__

#include <common.h>
#include "queue.h"

typedef void (*timeout_t)(void *);
typedef int64_t sbintime_t;

typedef struct callout {
    TAILQ_ENTRY(callout) c_link;
    sbintime_t c_time;  /* ticks to the event */
    timeout_t *c_func;  /* function to call */
    void    *c_arg;     /* function argument */
    uint16_t c_flags;
    //...
} callout_t;

#define CALLOUT_ACTIVE      0x0001 /* callout is currently active */
#define CALLOUT_PENDING     0x0002 /* callout is waiting for timeout */

#define callout_active(c)     B_SET((c)->c_flags, CALLOUT_ACTIVE)
#define callout_deactivate(c) B_CLR((c)->c_flags, CALLOUT_ACTIVE)

void callout_init();
void callout_setup(callout_t *handle, sbintime_t time, timeout_t fn, void *arg);
void callout_stop(callout_t *handle);

/* Process all timeouts, should be called from hardclock or softclock. */
void callout_process(sbintime_t now);



#endif /* __CALLOUT_H__ */
