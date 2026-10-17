#ifndef GAME_RESOURCE_CACHE_H
#define GAME_RESOURCE_CACHE_H

struct NamedResourceEntry;
struct PoolList;
struct ResourceProvider10;
struct ResourceProvider10VTable;

#include "game/pool_list.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct ResourceProvider10VTable {
    const void *reserved00[6];
    void *(*load)(struct ResourceProvider10 *provider, const char *name);
};

struct ResourceProvider10 {
    const struct ResourceProvider10VTable *vtable;
    unsigned char reserved04[0x0c];
    struct PoolList cache;
};

#ifdef __cplusplus
extern "C" {
#endif
void *func_001cc0dc(struct PoolList *cache, const char *name);
struct NamedResourceEntry *func_001cc050(struct PoolList *cache, const char *name,
                                 void *resource, unsigned char owned);
extern void *func_001cd2f0(struct ResourceProvider10 *provider, const char *name);
#ifdef __cplusplus
}
#endif

#endif
