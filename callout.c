#include "callout.h"
#include "libkern.h"

// The stuff in callout.h is just external interface. Here, we will do some magic.

#define NUMBER_OF_CALLOUT_BUCKETS 5

/*
  Every event is inside one of NUMBER_OF_CALLOUT_BUCKETS buckets.
  The buckets is a cyclic list, but we implement it as an array,
  thus allowing us to access random elements in constant time.

  TODO:
  - Change from unsigned int to some proper type. But if it's unsigned,
    make sure you don't decrease a zero.

  - long longs (nor int64_t) work: "Undefined reference to '__divdi3'"".
    https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html, so I had
    to change sbintime_t from int64_t to int.
    Do we have <stdint.h>? smallclib doesn't have it.

  - what to do with that strange bitset.h and _bitset.h?
    Edit an external source file?
    Besides, those bitset functions are weird

  - What do "pending" and "active" mean? What's all about these flags?

  - Keep information about time left (which basically means "ticks"), or simply time?
    The first one means that we have to decrement every callout. Then callout_process()
    shouldn't accept any argument.
    The second one means that we have to know the current time somehow during
    callout_setup, and not just the delta.

  - Who deallocates the memory for callouts? Does it happen
    during the function execution? Do I take care of that,
    or the person that registered the callout?
*/

TAILQ_HEAD(callout_head, callout);


typedef struct callout_internal {
  struct callout_head heads[NUMBER_OF_CALLOUT_BUCKETS];
  unsigned int current_position; /* Which of these heads is at current time. */
  unsigned int uptime; /* Number of ticks since the system started */
} callout_internal_t;

static callout_internal_t ci;


void callout_init() {
  memset(&ci, 0, sizeof ci);

  for (int i = 0; i < NUMBER_OF_CALLOUT_BUCKETS; i++)
    TAILQ_INIT(&ci.heads[i]);
}

void callout_setup(struct callout *handle, sbintime_t time, timeout_t fn, void *arg) {
  memset(handle, 0, sizeof(struct callout));

  int index = (time + ci.uptime) % NUMBER_OF_CALLOUT_BUCKETS;

  handle->c_time = time + ci.uptime;
  handle->c_func = fn;
  handle->c_arg = arg;
  handle->index = index;
  callout_activate(handle);

  log("Inserting into index: %d, because current_position = %d, time = %d, uptime = %d", index, ci.current_position, time, ci.uptime);
  TAILQ_INSERT_TAIL(&ci.heads[index], handle, c_link);
}

void callout_stop(callout_t *handle) {
  TAILQ_REMOVE(&ci.heads[handle->index], handle, c_link);
}

/* 
  If the time of an event comes, execute the callout function and delete it from the list. 
  Returns true if an element was deleted, false otherwise.
*/
bool process_element(struct callout_head* head, struct callout* element) {
  if (element->c_time == ci.uptime) {
    TAILQ_REMOVE(head, element, c_link);
    element->c_func(element->c_arg);
    return true;
  }

  if (element->c_time < ci.uptime) {
    panic("%s", "The time of a callout is smaller than uptime.");
  }

  return false;
}

/*
  This function makes a tick takes the next bucket and deals with its contents.
  If we want to run through several buckets at once, just run
  this function many times.
*/
void callout_process(sbintime_t now) {
  ci.current_position = (ci.current_position + 1) % NUMBER_OF_CALLOUT_BUCKETS;
  ci.uptime++;

  struct callout_head* head = &ci.heads[ci.current_position];
  struct callout* current;

  TAILQ_FOREACH(current, head, c_link) {
    // Deal with the next element if the currrent one is not the tail.
    bool element_deleted;
    do {
      element_deleted = false;

      if (current != TAILQ_LAST(head, callout_head)) {
        struct callout* next = TAILQ_NEXT(current, c_link);
        element_deleted = process_element(head, next);
      }
    } while (element_deleted);
  }

  // Deal with the first element
  if (!TAILQ_EMPTY(head)) {
    struct callout* first = TAILQ_FIRST(head);
    //log("Trying to process the head");
    process_element(head, first);
  }
}