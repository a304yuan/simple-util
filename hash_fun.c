#include "hash_fun.h"

/* FNV 1a 64-bit Hash */

size_t fnv_hash64(const void *key, size_t len) {
    uint64_t h = 0xcbf29ce484222325ul;
    const unsigned char *k = key;

    for(size_t i = 0; i < len; i++) {
        h ^= k[i];
        h *= 0x100000001b3ul;
    }

    return (size_t)h;
}

/* BKDR hash */

size_t bkdr_hash(const void *key, size_t len) {
    size_t h = 0;
    const unsigned char *k = key;

    for(size_t i = 0; i < len; i++)
        h = h * 131 + k[i];

    return h;
}

/* RS hash */

size_t rs_hash(const void *key, size_t len) {
    size_t h = 0;
    size_t magic = 63689u;
    const unsigned char *k = key;

    for(size_t i = 0; i < len; i++) {
        h = h * magic + k[i];
        magic *= 378551u;
    }

    return h;
}

/* AP hash */

size_t ap_hash(const void *key, size_t len) {
    size_t h = 0;
    const unsigned char *k = key;

    for(size_t i = 0; i < len; i++) {
        if (i & 1)
            h ^= (~((h << 11) ^ k[i] ^ (h >> 5)));
        else
            h ^= ((h << 7) ^ k[i] ^ (h >> 3));
    }

    return h;
}

/* JS hash */

size_t js_hash(const void *key, size_t len) {
    size_t h = 1315423911u;
    const unsigned char *k = key;

    for(size_t i = 0; i < len; i++)
        h ^= ((h << 5) + k[i] + (h >> 2));

    return h;
}

/* DEK hash */

size_t dek_hash(const void *key, size_t len) {
    size_t h = 1315423911u;
    const unsigned char *k = key;

    for(size_t i = 0; i < len; i++)
        h = ((h << 5) ^ (h >> 27)) ^ k[i];

    return h;
}

/* DJB hash */

size_t djb_hash(const void *key, size_t len) {
    size_t h = 5381u;
    const unsigned char *k = key;

    for(size_t i = 0; i < len; i++)
        h += (h << 5) + k[i];

    return h;
}

/* PJW hash */
size_t pjw_hash(const void *key, size_t len)  {
    size_t bits = sizeof(size_t) * 8;
    size_t three_four = (bits  * 3) / 4;
    size_t one_eight = bits / 8;
    size_t high_bits = ((size_t)-1) << (bits - one_eight);

    size_t h = 0;
    size_t magic = 0;
    const unsigned char *k = key;

    for(size_t i = 0; i < len; i++) {
        h = (h << one_eight) + k[i];
        if ((magic = h & high_bits) != 0)
            h = ((h ^ (magic >> three_four)) & (~high_bits));
    }

    return h;
}

/* ELF hash */
size_t elf_hash(const void *key, size_t len)  {
    size_t bits = sizeof(size_t) * 8;
    size_t three_four = (bits  * 3) / 4;
    size_t one_eight = bits / 8;
    size_t high_bits = ((size_t)-1) << (bits - one_eight);

    size_t h = 0;
    size_t magic = 0;
    const unsigned char *k = key;

    for(size_t i = 0; i < len; i++) {
        h = (h << one_eight) + k[i];
        if ((magic = h & high_bits) != 0) {
            h ^= (magic >> three_four);
            h &= ~magic;
        }
    }

    return h;
}
