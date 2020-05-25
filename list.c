#include <stdlib.h>
#include "list.h"

list * list_new() {
    list * l = malloc(sizeof(list));
    l->size = 0;
    l->node = NULL;
    return l;
}

void list_free(list * lt) {
    list_node * n = lt->node;
    while (n) {
        list_node * t = n->next;
        free(n);
        n = t;
    }
    free(lt);
}

void list_add(list * lt, any val) {
    list_node * n = malloc(sizeof(list_node));
    n->val = val;
    n->next = NULL;

    list_node ** p = &lt->node;
    while (*p) {
        p = &(*p)->next;
    }
    *p = n;
    lt->size++;
}

any list_get_head(list * lt) {
    any val;
    if (lt->node) {
        val = lt->node->val;
        lt->node = lt->node->next;
        lt->size--;
        return val;
    }
    else {
        return ANY_NONE;
    }
}

any list_get_tail(list * lt) {
    any val = ANY_NONE;
    list_node ** n = &lt->node;
    while (*n) {
        if ((*n)->next) {
            n = &(*n)->next;
        }
        else {
            val = (*n)->val;
            free(*n);
            *n = NULL;
            lt->size--;
        }
    }
    return val;
}