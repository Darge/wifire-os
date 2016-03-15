#include "callout.h"
#include "libkern.h"

// The stuff in callout.h is just external interface. Here, we will do some magic.

#define NUMBER_OF_CALLOUT_BUCKETS 200

/*
  Every event is inside one of NUMBER_OF_CALLOUT_BUCKETS buckets.
  The buckets is a cyclic list, but we implement it as an array,
  thus allowing us to access random elements in constant time.

  TODO:
  -Change from unsigned int to some proper type.
  - long longs (nor int64_t) work: "Undefined reference to '__divdi3'"".
    https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html, so I had
    to change sbintime_t from int64_t to int.
    Do we have <stdint.h>? smallclib doesn't have it.
  - what to do with that strange bitset.h and _bitset.h?
    Edit an external source file?
    Besides, those bitset functions are weird
  - What do "pending" and "active" mean?
  - "Let’s not take a saw to the branch we're sitting on".
    How to delete elements from the list?
*/

TAILQ_HEAD(callout_head, callout);


typedef struct callout_internal {
  struct callout_head heads[NUMBER_OF_CALLOUT_BUCKETS];
  unsigned int current_position; /* Which of these heads is at current time. */
} callout_internal_t;

static callout_internal_t ci;


void callout_init() {
  memset(&ci, 0, sizeof ci); 

  for (int i = 0; i < NUMBER_OF_CALLOUT_BUCKETS; i++)
    TAILQ_INIT(&ci.heads[i]);
}

void callout_setup(struct callout *handle, sbintime_t time, timeout_t fn, void *arg) {
  memset(handle, 0, sizeof(struct callout)); 
  
  int index = (ci.current_position + time) % NUMBER_OF_CALLOUT_BUCKETS;

  handle->c_time = time;
  handle->c_func = fn;
  handle->c_arg = arg;
  handle->index = index;
  callout_active(handle);

  TAILQ_INSERT_HEAD(&ci.heads[index], handle, c_link);

  uint64_t abc = 0; // this works though.
  abc++;
}

void callout_stop(callout_t *handle) {
  TAILQ_REMOVE(&ci.heads[handle->index], handle, c_link);
}

void process_element(struct callout* element) {
  // If the time has come, execute it and delete from the list.
}

/* Process all timeouts, should be called from hardclock or softclock. */
/* This function takes the next bucket and deals with its contents. */
void callout_process(sbintime_t now) {
  ci.current_position = (ci.current_position + 1) % NUMBER_OF_CALLOUT_BUCKETS;

  struct callout_head head = ci.heads[ci.current_position];
  struct callout_head newHead;
  TAILQ_INIT(&newHead);


  struct callout* current;
  TAILQ_FOREACH(current, &head, c_link) {
    // We deal with the next element if the currrent one is not the tail.
    if (current != TAILQ_LAST(&head, callout_head)) { // Is this proper?
      struct callout* next = TAILQ_NEXT(current, c_link);
      process_element(next);
    }
  }

  // We deal with the first element
  if (!TAILQ_EMPTY(&head)) {
    struct callout* first = TAILQ_FIRST(&head);
    process_element(first);
  }
}
