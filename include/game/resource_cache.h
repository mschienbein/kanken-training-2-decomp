#ifndef GAME_RESOURCE_CACHE_H
#define GAME_RESOURCE_CACHE_H

struct NamedResourceEntry;
struct PoolList;
struct ResourceProvider10;
struct ResourceProvider10VTable;

#include "game/pool_list.h"
#include "game/runtime.h"

/* [INFERENCE] Existing provider/cache roles and retained matching layout. */
struct ResourceProvider10VTable {
    const void *reserved00[6];
    void *(*load)(struct ResourceProvider10 *provider, const char *name);
};
struct ResourceProvider10 {
    const struct ResourceProvider10VTable *vtable;
    unsigned char reserved04[0x0c];
    struct PoolList cache;
};

/* [INFERENCE] Cache/value roles. Only the recovered fixed prefix is named. */
struct CacheEntry {
    unsigned first, second, third, fourth;
#ifdef __cplusplus
    CacheEntry(unsigned a, unsigned b, unsigned c, unsigned d)
        : first(a), second(b), third(c), fourth(d) {}
#endif
};
struct CacheVTable;
struct ResourceCache {
    const struct CacheVTable *vtable;
    unsigned char active;
    unsigned char reserved05[3];
    unsigned count;
    struct CacheEntry entries[4];
};
#ifdef __cplusplus
extern "C" {
#endif
void *func_001cc0dc(struct PoolList *cache, const char *name);
struct NamedResourceEntry *func_001cc050(struct PoolList *cache, const char *name,
                                       void *resource, unsigned char owned);
extern void *func_001cd2f0(struct ResourceProvider10 *provider, const char *name);
extern const struct CacheVTable data_00223778;
struct ResourceCache *func_001c19e0(struct ResourceCache *cache);
#ifdef __cplusplus
}
#endif
#endif
