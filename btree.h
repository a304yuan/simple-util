#ifndef SIMPLE_BTREE_H
#define SIMPLE_BTREE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "any.h"

#define BTREE_T 32

typedef struct btree_node btree_node;

struct btree_node {
	int keys;
    int childs;
	int idx;
	any key[2 * BTREE_T - 1];
    any value[2 * BTREE_T - 1];
	btree_node * parent;
	btree_node * child[2 * BTREE_T];
};

typedef struct btree btree;
struct btree {
	size_t nodes;
	size_t keys;
	int (*compare)(const any *, const any *);
	btree_node * root;
};

extern btree * btree_new(int (*compare)(const any *, const any *));
extern void btree_free(btree * tree);
static inline size_t btree_nodes(const btree * tree) {
    return tree->nodes;
}
static inline size_t btree_keys(const btree * tree) {
    return tree->keys;
}
extern bool btree_exist(const btree * tree, any key);
extern any btree_get(const btree * tree, any key);
extern void btree_insert(btree * tree, any key, any value);
extern void btree_delete(btree * tree, any key);

#endif // SIMPLE_BTREE_H
