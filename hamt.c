#include "hamt.h"

static int _primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59};

static hamt_array_node * hamt_array_node_new(int level) {
    int size = level >= 0 ? _primes[level] : _primes[0];
    hamt_array_node * n = malloc(sizeof(hamt_array_node) + sizeof(hamt_node*) * size);
    n->base.type = ARRAY_NODE;
    n->size = size;
    n->level = level;
    for (int i = 0; i < size; i++) {
        n->nodes[i] = NULL;
    }
    return n;
}

static void hamt_array_node_free(hamt_array_node * arrn) {
    for (int i = 0; i < arrn->size; i++) {
        hamt_node * n = arrn->nodes[i];
        if (n) {
            if (n->type == KV_NODE) {
                free(n);
            }
            else {
                hamt_array_node_free((hamt_array_node*)n);
            }
        }
    }
    free(arrn);
}

hamt * hamt_new(size_t (*hash_fun)(const any *), int (*compare)(const any *, const any *)) {
    hamt * h = malloc(sizeof(hamt));
    h->size = 0;
    h->hash_fun = hash_fun;
    h->compare = compare;
    h->root = hamt_array_node_new(16);
    return h;
}

void hamt_free(hamt * hm) {
    free(hm->root);
    free(hm);
}

void hamt_put(hamt * hm, any key, any value) {
    size_t hash = hm->hash_fun(&key);
    hamt_array_node * p = hm->root;
    hamt_node ** n = NULL;
    while (1) {
        n = p->nodes + hash % p->size;
        if ((*n) == NULL) {
            hamt_kv_node * newn = malloc(sizeof(hamt_kv_node));
            newn->base.type = KV_NODE;
            newn->hash = hash;
            newn->key = key;
            newn->value = value;
            *n = (hamt_node*)newn;
            break;
        }
        else if ((*n)->type == ARRAY_NODE) {
            p = (hamt_array_node*)(*n);
        }
        else {
            hamt_kv_node * _n = (hamt_kv_node*)(*n);
            if (_n->hash == hash && hm->compare(&_n->key, &key) == 0) {
                _n->value = value;
                break;
            }
            else {
                hamt_array_node * arr = hamt_array_node_new(p->level - 1);
                arr->nodes[_n->hash % arr->size] = (hamt_node*)_n;
                *n = (hamt_node*)arr;
                p = arr;
            }
        }
    }
}

any hamt_get(const hamt * hm, any key) {
    size_t hash = hm->hash_fun(&key);
    hamt_array_node * p = hm->root;
    hamt_kv_node * kv = NULL;
    hamt_node * n = NULL;
    while (p) {
        n = p->nodes[hash % p->size];
        if (n == NULL) {
            break;
        }
        else if (n->type == ARRAY_NODE) {
            p = (hamt_array_node*)n;
        }
        else {
            hamt_kv_node * _n = (hamt_kv_node*)n;
            if (hash == _n->hash && hm->compare(&_n->key, &key) == 0) {
                kv = _n;
            }
            break;
        }
    }
    return kv ? kv->value : ANY_NONE;
}
