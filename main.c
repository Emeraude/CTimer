#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "timer.h"

static void __test(void *data) {
  printf("%s\n", (char *)data);
}

int main(void) {
  if (new_timer(__test, "42", 1000, TIMER_SEVERAL | TIMER_CALL_BEFORE) == -1
      || new_timer(__test, "84", 2000, TIMER_SEVERAL) == -1)
    return 1;
  sleep(5);
  return 0;
}
