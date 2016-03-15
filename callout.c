#include "callout.h"
#include "libkern.h"

// The stuff in callout.h is just external interface. Here, we will do some magic.

#define NUMBER_OF_CALLOUT_BUCKETS 200

/*
  Every event is inside one of NUMBER_OF_CALLOUT_BUCKETS buckets.
  The buckets is a cyclic list, but we implement it as an array,
  thus allowing us to access random elements in constant time.

  TODO: Change from unsigned int to some proper type.
*/

TAILQ_HEAD(callout_head, callout);


typedef struct callout_internal {
  struct callout_head heads[NUMBER_OF_CALLOUT_BUCKETS];
  unsigned int current_position; /* Which of these heads is at current time. */
} callout_internal_t;

static callout_internal_t ci;


void callout_init()
{
  memset(&ci, 0, sizeof ci); 
  //ci.current_position = 0;
  for (int i = 0; i < NUMBER_OF_CALLOUT_BUCKETS; i++)
  {
    TAILQ_INIT(&ci.heads[i]);
  }

}

void callout_setup(struct callout *handle, sbintime_t time, timeout_t fn, void *arg)
{
  // Calculate index at which we should put that callout.
  int index = (ci.current_position + time) % NUMBER_OF_CALLOUT_BUCKETS;
  //struct callout_head* head = ci.heads[index];

  TAILQ_INSERT_HEAD(&ci.heads[index], handle, c_link);

  //TAILQ_FIRST(&head);

  //TAILQ_INSERT_HEAD(&(ci.heads[index]), handle, c_link);


}

void callout_stop(callout_t *handle)
{

}

/* Process all timeouts, should be called from hardclock or softclock. */
void callout_process(sbintime_t now)
{

}
