#ifndef GAME_POOL_LIST_H
#define GAME_POOL_LIST_H

struct PoolLink {
    struct PoolLink *next;
    struct PoolLink *previous;
};

struct PoolList {
    unsigned int count;
    struct PoolLink sentinel;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct PoolLink *func_00191154(struct PoolList *list, struct PoolLink *position,
                                     struct PoolLink *node);
extern struct PoolLink *func_00191178(struct PoolList *list, struct PoolLink *first);
#ifdef __cplusplus
}
#endif

#endif
