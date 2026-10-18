#ifndef GAME_RESOURCE_CACHE_H
#define GAME_RESOURCE_CACHE_H
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
extern const struct CacheVTable data_00223778;
struct ResourceCache *func_001c19e0(struct ResourceCache *cache);
#ifdef __cplusplus
}
#endif
#endif
