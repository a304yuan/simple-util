#ifndef HASH_FUN_H
#define HASH_FUN_H

#include <stdlib.h>
#include <stdint.h>

extern size_t fnv_hash64(const void *key, size_t len);
extern size_t bkdr_hash(const void *key, size_t len);
extern size_t rs_hash(const void *key, size_t len);
extern size_t ap_hash(const void *key, size_t len);
extern size_t js_hash(const void *key, size_t len);
extern size_t dek_hash(const void *key, size_t len);
extern size_t djb_hash(const void *key, size_t len);
extern size_t pjw_hash(const void *key, size_t len);
extern size_t elf_hash(const void *key, size_t len);

#endif // HASH_FUN_H
