#ifndef SIMPLE_HASH_MAP_H
#define SIMPLE_HASH_MAP_H

#include <stdlib.h>
#include <stdint.h>
#include "any.h"

typedef struct hash_map_node hash_map_node;
typedef struct hash_map_node_array hash_map_node_array;
typedef struct hash_map hash_map;
typedef struct hash_map_iter hash_map_iter;

enum hash_map_node_status {
    EMPTY = 0,
    USED,
    DELETED
};

struct hash_map_node {
    int status;
    size_t hash;
    any key;
    any value;
};

struct hash_map_node_array {
    size_t size;
    size_t capacity;
    hash_map_node * nodes;
};

struct hash_map {
    uint64_t seed;
    size_t (*hash_fun)(const any *);
    int (*compare)(const any *, const any *);
    hash_map_node_array array[2];
};

struct hash_map_iter {
    const hash_map * map;
    const hash_map_node * next_node;
    const hash_map_node_array * array;
    bool (*has_next)(hash_map_iter * iter);
    const hash_map_node * (*next)(hash_map_iter * iter);
};

extern hash_map * hash_map_new(size_t size, size_t (*hash_fun)(const any *), int (*compare)(const any *, const any *));
extern void hash_map_free(hash_map * map);
extern void hash_map_put(hash_map * map, any key, any value);
extern any hash_map_get(const hash_map * map, any key);
extern any hash_map_delete(hash_map * map, any key);

extern void hash_map_iter_init(hash_map_iter * iter, const hash_map * map);

#endif