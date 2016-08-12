#include <mutex.h>
#include <interrupts.h>

volatile int cs_level = 0;;

void cs_enter() {
  intr_disable();
  cs_level++;
}

void cs_leave() {
  assert(cs_level > 0);

  cs_level--;
  if (cs_level == 0)
    intr_enable();
}
