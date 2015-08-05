#pragma once

#include <time.h>

#define TIMER_SEVERAL 1
#define TIMER_CALL_BEFORE 2

int new_timer(void (*fptr)(void *), void *data, const int ms, const int flags);
void delete_timer(void (*fptr)(void *), void *data);
