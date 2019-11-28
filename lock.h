#ifndef LOCK_H
#define LOCK_H

#include <stdlib.h>
#include <stdatomic.h>

/**
 * Spin lock
 */
typedef atomic_flag spinlock;
extern void spinlock_init(spinlock *lock);
extern void spinlock_lock(spinlock *lock);
extern void spinlock_unlock(spinlock *lock);

/**
 * RW lock
 */
 enum rw_stat {
     READING = 0,
     IDLE,
     WRITING
 };

typedef struct rw_lock_struct {
    _Atomic long r_count;
    _Atomic int status;
} rwlock_t;

void rwlock_init(rwlock_t *lock);
void rwlock_rlock(rwlock_t *lock);
void rwlock_runlock(rwlock_t *lock);
void rwlock_wlock(rwlock_t *lock);
void rwlock_wunlock(rwlock_t *lock);

/**
 * Sequence lock
 */
typedef struct {
	size_t sequence;
	spinlock lock;
} seqlock_t;

void seqlock_init(seqlock_t *seql);
void seqlock_wlock(seqlock_t *seql);
void seqlock_wunlock(seqlock_t *seql);
size_t seqlock_read(seqlock_t *seql);
int seqlock_read_retry(seqlock_t *seql, size_t seq);

#endif // LOCK_H
