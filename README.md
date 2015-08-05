# CTimer

CTimer is an abstraction for POSIX pre-process timers.  

## Synopsis

```C
int new_timer(void (*fptr)(void *), void *data, const int ms, const int flags);
void delete_timer(void (*fptr)(void *), void *data);
```

## Description

**new_timer** create a timer that will call the function *fptr* after *ms* milliseconds and pass *data* as parameter of *fptr*. If you pass **TIMER_SEVERAL** in *flags*, the timer will be re-called after *ms* milliseconds indefinitely. You can also pass **TIMER_CALL_BEFORE** to also call the function once immediately.  
**delete_timer** will delete the timer that is defined by the function *fptr* and the argument *data*.

## Notes

If you think some leak occur, that is because the first call to **new_timer** create a thread, that will not be freed after the end of the timer. It's always the same thread that is used, and we do not have to destroy it, so some leak, (less than 1k) may occur.

### Author

Emeraude
