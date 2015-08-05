#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.h"
#include "timer.h"

typedef struct {
  int flags;
  struct itimerspec its;
  timer_t timerid;
  void (*fptr)(void *);
  void *data;
} t_otimer;

static t_list *timer_list = NULL;

static void __handler(union sigval sigev) {
  t_otimer *ot = sigev.sival_ptr;

  ot->fptr(ot->data);
  if (ot->flags & TIMER_SEVERAL) {
    if (timer_settime(ot->timerid, 0, &ot->its, NULL)) {
      perror("timer_settime");
      return;
    }
  }
  else {
    if (timer_delete(ot->timerid)) {
      perror("timer_delete");
      return ;
    }
    free(ot);
  }
}

static void __delete_list(void) {
  list_delete(timer_list);
}

static void __hook_delete(void *const data) {
  if (timer_delete(((t_otimer *)data)->timerid))
    perror("timer_delete");
  free(data);
}

static int __hook_cmp(void const *const a, void const *const b) {
  return ((t_otimer *)a)->fptr == ((t_otimer *)b)->fptr
    && ((t_otimer *)a)->data == ((t_otimer *)b)->data;
}

int new_timer(void (*fptr)(void *), void *data, const int ms, const int flags) {
  t_otimer *ot;
  struct sigevent sevp;

  if (!timer_list) {
    if (!(timer_list = list_new()))
      return -1;
    timer_list->pop = __hook_delete;
    timer_list->cmp = __hook_cmp;
    atexit(__delete_list);
  }
  if (!(ot = malloc(sizeof(*ot)))) {
    perror("malloc");
    return -1;
  }
  memset(ot, 0, sizeof(*ot));
  ot->flags = flags;
  ot->fptr = fptr;
  ot->data = data;
  if (flags & TIMER_SEVERAL)
    if (!list_push_back(timer_list, ot))
      return -1;
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

void delete_timer(void (*fptr)(void *), void *data) {
  t_otimer ot;

  ot.fptr = fptr;
  ot.data = data;
  list_pop_search(timer_list, &ot);
}
