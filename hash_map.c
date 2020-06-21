#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "hash_map.h"
#include "random.h"

static inline size_t prob(size_t hash, size_t count) {
    return hash + count >> 1 + (count * count) >> 1;
}

static inline bool _hash_map_is_expanding(const hash_map * map) {
    return map->array[1].capacity > 0;
}

static inline bool _hash_map_node_equal(const hash_map * map, const hash_map_node * node, size_t hash, const any * key) {
    return node->hash == hash && map->compare(&node->key, key) == 0;
}

static void _hash_map_put(hash_map * map, hash_map_node_array * array, size_t hash, const any * key, const any * value) {
    size_t count = 0;
    while (true) {
        size_t idx = prob(hash, count) % array->capacity;
        hash_map_node * node = array->nodes + idx;
        if (node->status != USED) {
            node->status = USED;
            node->hash = hash;
            node->key = *key;
            node->value = *value;
            array->size++;
            break;
        }
        else if (_hash_map_node_equal(map, node, hash, key)) {
            node->value = *value;
            break;
        }
        else {
            count++;
        }
    }
}

static hash_map_node * _hash_map_get(const hash_map * map, const hash_map_node_array * array, size_t hash, const any * key) {
    size_t count = 0;
    while (true) {
        size_t idx = prob(hash, count) % array->capacity;
        hash_map_node * node = array->nodes + idx;
        if (node->status == EMPTY) {
            return NULL;
        }
        else if (_hash_map_node_equal(map, node, hash, key)) {
            return node->status == USED ? node : NULL;
        }
        else {
            count++;
        }
    }
}

static any _hash_map_delete(hash_map * map, hash_map_node_array * array, size_t hash, const any * key) {
    hash_map_node * node = NULL;
    any v;
    node = _hash_map_get(map, array, hash, key);
    if (node) {
        node->status = DELETED;
        v = node->value;
        array->size--;
    }
    else {
        v = ANY_NONE;
    }
    return v;
}

hash_map * hash_map_new(size_t size, size_t (*hash_fun)(const any *), int (*compare)(const any *, const any *)) {
    hash_map * map = malloc(sizeof(hash_map));
    map->seed = time(NULL);
    map->hash_fun = hash_fun;
    map->compare = compare;
    hash_map_node * nodes = calloc(1, sizeof(hash_map_node) * size);
    //memset(nodes, 0, sizeof(hash_map_node) * size);
    map->array[0] = (hash_map_node_array){.size = 0, .capacity = size, .nodes = nodes};
    map->array[1] = (hash_map_node_array){.size = 0, .capacity = 0, .nodes = NULL};
    return map;
}

void hash_map_free(hash_map * map) {
    free(map->array[0].nodes);
    if (map->array[1].nodes) free(map->array[1].nodes);
    free(map);
}

void hash_map_put(hash_map * map, any key, any value) {
    size_t hash = map->hash_fun(&key);

    if (_hash_map_is_expanding(map)) {
        _hash_map_delete(map, &map->array[0], hash, &key);
        _hash_map_put(map, &map->array[1], hash, &key, &value);

        // move one node from old array to new one
        if (map->array[0].size > 0) {
            size_t start = xorshift64(&map->seed);
            for (size_t i = 0; i < map->array[0].capacity; i++) {
                size_t idx = (start + i) % map->array[0].capacity;
                hash_map_node * old = map->array[0].nodes + idx;
                if (old->status == USED) {
                    _hash_map_put(map, &map->array[1], old->hash, &old->key, &old->value);
                    map->array[0].size--;
                    old->status = DELETED;
                    break;
                }
            }
        }
        else {
            free(map->array[0].nodes);
            map->array[0] = map->array[1];
            map->array[1] = (hash_map_node_array){.size = 0, .capacity = 0, .nodes = NULL};
        }
    }
    else {
        _hash_map_put(map, &map->array[0], hash, &key, &value);

        // expand capacity to double times if load factor is greater than 0.7
        if ((double)(map->array[0].size) / map->array[0].capacity >= 0.6) {
            size_t cap = map->array[0].capacity << 1;
            hash_map_node * n = calloc(1, sizeof(hash_map_node) * cap);
            //memset(n, 0, sizeof(hash_map_node) * cap);
            map->array[1] = (hash_map_node_array){.size = 0, .capacity = cap, .nodes = n};
        }
    }
}

any hash_map_get(const hash_map * map, any key) {
    size_t hash = map->hash_fun(&key);
    hash_map_node * node = NULL;
    if (_hash_map_is_expanding(map)) {
        node = _hash_map_get(map, &map->array[1], hash, &key);
    }
    else if (!node) {
        node = _hash_map_get(map, &map->array[0], hash, &key);
    }
    return node ? node->value : ANY_NONE;
}

any hash_map_delete(hash_map * map, any key) {
    size_t hash = map->hash_fun(&key);
    hash_map_node * node;
    any v = ANY_NONE;
    if (_hash_map_is_expanding(map)) {
        v = _hash_map_delete(map, &map->array[1], hash, &key);
    }
    if (any_is_none(&v)) {
        v = _hash_map_delete(map, &map->array[0], hash, &key);
    }
    return v;
}

static bool hash_map_iter_has_next(hash_map_iter * iter) {
    while (iter->next_node < iter->array->nodes + iter->array->capacity) {
        if (iter->next_node->status != USED) {
            iter->next_node += 1;
        }
        else {
            return true;
        }
    }
    if (iter->array == iter->map->array) {
        iter->array += 1;
        iter->next_node = iter->array->nodes;
        return hash_map_iter_has_next(iter);
    }
    else {
        return false;
    }
}

static const hash_map_node * hash_map_iter_next(hash_map_iter * iter) {
    return iter->next_node++;
}

void hash_map_iter_init(hash_map_iter * iter, const hash_map * map) {
    iter->map = map;
    iter->array = map->array;
    iter->next_node = map->array[0].nodes;
    iter->has_next = hash_map_iter_has_next;
    iter->next = hash_map_iter_next;
}