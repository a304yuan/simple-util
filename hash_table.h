#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "any.h"

typedef struct hash_node hash_node;
typedef struct hash_table hash_table;
typedef struct hash_table_iter hash_table_iter;

struct hash_node {
    size_t hash;       //!< Hash value
    size_t count;      //!< Occurences
    any key;           //!< key
    any value;         //!< value
    hash_node * next;   //!< Pointer to next node
    hash_node * order_prev;
    hash_node * order_next;
};

struct hash_table {
    size_t slots[2];         //!< Number of slots
    size_t nodes;            //!< Number of nodes
    size_t (*hash_fun)(const any *);  //!< Pointer to a hash function
    int (*compare)(const any *, const any *);
    hash_node ** array[2];    //!< [0] Currently used array of node pointers [1] Expanded array of node pointers
    hash_node * last;
};

struct hash_table_iter {
    hash_node * next;
};

static inline void hash_table_iter_init(const hash_table * table, hash_table_iter * iter) {
    iter->next = table->last;
}

static inline bool hash_table_iter_has(const hash_table_iter * iter) {
    return iter->next != NULL;
}

static inline hash_node * hash_table_iter_next(hash_table_iter * iter) {
    hash_node * n = iter->next;
    iter->next = n->order_prev;
    return n;
}

extern hash_table * hash_table_new(size_t capacity, size_t (*hash_fun)(const any *), int (*compare)(const any *, const any *));
extern void hash_table_free(hash_table * table, void (*deallocator)(any * key, any * value));
extern hash_node * hash_table_insert(hash_table * table, const any key, const any value);
extern void hash_table_delete(hash_table * table, const any key);
extern hash_node * hash_table_find(const hash_table * table, const any key);
extern any hash_table_get(const hash_table * table, const any key);

#endif // HASH_TABLE_H
