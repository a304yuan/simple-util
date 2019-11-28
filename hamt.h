#ifndef HAMT_H
#define HAMT_H

#include <stdlib.h>
#include "any.h"

typedef struct hamt hamt;
typedef struct hamt_node hamt_node;
typedef struct hamt_array_node hamt_array_node;
typedef struct hamt_kv_node hamt_kv_node;

enum hamt_node_type {
    KV_NODE,
    ARRAY_NODE
};

struct hamt_node {
    enum hamt_node_type type;
};

struct hamt_array_node {
    hamt_node base;
    int size;
    int level;
    hamt_node * nodes[];
};

struct hamt_kv_node {
    hamt_node base;
    size_t hash;
    any key;
    any value;
};

struct hamt {
    size_t size;
    size_t (*hash_fun)(const any * key);
    int (*compare)(const any *, const any *);
    hamt_array_node * root;
};

extern hamt * hamt_new(size_t (*hash_fun)(const any *), int (*compare)(const any *, const any *));
extern void hamt_free(hamt * hm);
extern void hamt_put(hamt * hm, any key, any value);
extern any hamt_get(const hamt * hm, any key);

#endif /* end of include guard: HAMT_H */
