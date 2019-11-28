#define _GNU_SOURCE
#include <stdlib.h>
#include "thread.h"

static int thread_main_func(void * arg) {
    thread * thrd = arg;
    int ret = thrd->func(thrd->arg);
    free(thrd);
    return ret;
}

void thread_create(thread_func func, void * arg) {
    thread * thrd = malloc(sizeof(thread));
    thrd->func = func;
    thrd->arg = arg;
    clone(
        thread_main_func,
        thrd->stack + THREAD_STACK_SIZE,
        CLONE_VM | CLONE_THREAD | CLONE_SIGHAND | CLONE_FS | CLONE_IO | CLONE_FILES | CLONE_PARENT,
        thrd
    );
}
