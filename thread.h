#ifndef SIMPLE_THREAD_H
#define SIMPLE_THREAD_H

#include <unistd.h>
#include <sched.h>

#define THREAD_STACK_SIZE 4096

typedef struct thread thread;
typedef int (*thread_func)(void * arg);

struct thread {
    thread_func func;
    void * arg;
    char stack[THREAD_STACK_SIZE];
};

extern void thread_create(thread_func func, void * arg);

#define thread_yield() sched_yield()

#define thread_sleep(secs) sleep(secs)

#endif /* end of include guard: SIMPLE_THREAD_H */
