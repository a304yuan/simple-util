#include "lock.h"

/**
 * Implementation of Spin lock
 */

static atomic_flag init_flag = ATOMIC_FLAG_INIT;

void spinlock_init(spinlock *lock) {
    *lock = init_flag;
}

void spinlock_lock(spinlock *lock) {
    while(atomic_flag_test_and_set_explicit(lock, memory_order_acquire));
}

void spinlock_unlock(spinlock *lock) {
    atomic_flag_clear_explicit(lock, memory_order_release);
}

/**
 * Implementation of RW lock
 */

void rwlock_init(rwlock_t *lock) {
    atomic_init(&lock->r_count, 0);
    atomic_init(&lock->status, IDLE);
}

void rwlock_rlock(rwlock_t *lock) {
    atomic_fetch_add(&lock->r_count, 1);
    int exp = IDLE;
    while(!atomic_compare_exchange_weak(&lock->status, &exp, READING)) {
        if (exp == READING) break;
        else exp = IDLE;
    }
}

void rwlock_runlock(rwlock_t *lock) {
    int exp = atomic_fetch_sub(&lock->r_count, 1) - 1;
    atomic_compare_exchange_strong(&lock->status, &exp, IDLE);
}

void rwlock_wlock(rwlock_t *lock) {
    int exp = IDLE;
    while(!atomic_compare_exchange_weak(&lock->status, &exp, WRITING)) {
        exp = IDLE;
    }
}

void rwlock_wunlock(rwlock_t *lock) {
    atomic_store(&lock->status, IDLE);
}

/**
 * Implementation of Sequence lock
 */

void seqlock_init(seqlock_t *seql) {
	seql->sequence = 0;
	spinlock_init(&seql->lock);
}

void seqlock_wlock(seqlock_t *seql) {
    spinlock_lock(&seql->lock);
    ++seql->sequence;
}

void seqlock_wunlock(seqlock_t *seql) {
    ++seql->sequence;
    spinlock_unlock(&seql->lock);
}

size_t seqlock_read(seqlock_t *seql) {
    size_t seq;
    do {
        seq = seql->sequence;
    }
    while(seq & 1);

    return seq;
}

int seqlock_read_retry(seqlock_t *seql, size_t seq) {
    atomic_thread_fence(memory_order_release);
    return seql->sequence != seq;
}
