#ifndef LIST_H
#define LIST_H

#include "any.h"

typedef struct list list;
typedef struct list_node list_node;

struct list_node {
    any val;
    list_node * next;
};

struct list {
    long size;
    list_node * node;
};

extern list * list_new();
extern void list_free(list * lt);
extern void list_add(list * lt, any val);
extern any list_get_head(list * lt);
extern any list_get_tail(list * lt);

static inline long list_size(const list * lt) {
    return lt->size;
}

#endif