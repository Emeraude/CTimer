#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "timer.h"

typedef struct {
  int flags;
  struct itimerspec its;
  timer_t timerid;
  void (*fptr)(void *);
  void *data;
} t_otimer;

static void __handler(union sigval sigev) {
  t_otimer *ot = sigev.sival_ptr;

  ot->fptr(ot->data);
  if (ot->flags & TIMER_SEVERAL) {
    if (timer_settime(ot->timerid, 0, &ot->its, NULL)) {
      perror("timer_settime");
      exit(1);
    }
  }
  else {
    if (timer_delete(ot->timerid)) {
      perror("timer_delete");
      exit(1);
    }
    free(ot);
  }
}

int new_timer(void (*fptr)(void *), void *data, const int ms, const int flags) {
  t_otimer *ot;
  struct sigevent sevp;

  if (!(ot = malloc(sizeof(*ot)))) {
    perror("malloc");
    return -1;
  }
  memset(ot, 0, sizeof(*ot));
  ot->flags = flags;
  ot->fptr = fptr;
  ot->data = data;
  memset(&sevp, 0, sizeof(sevp));
  sevp.sigev_notify = SIGEV_THREAD;
  sevp.sigev_value.sival_ptr = ot;
  sevp.sigev_notify_function = __handler;
  if (timer_create(CLOCK_MONOTONIC, &sevp, &ot->timerid)) {
    perror("timer_create");
    return -1;
  }
  ot->its.it_value.tv_sec = ms / 1000;
  ot->its.it_value.tv_nsec = ms % 1000 * 1000000;
  if (timer_settime(ot->timerid, 0, &ot->its, NULL)) {
    perror("timer_settime");
    return -1;
  }
  if (flags & TIMER_CALL_BEFORE)
    ot->fptr(ot->data);
  return 0;
}

