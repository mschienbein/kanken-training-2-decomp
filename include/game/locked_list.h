#ifndef GAME_LOCKED_LIST_H
#define GAME_LOCKED_LIST_H

struct LockedListOwner;
struct LockedListVTable;
struct ManagerLock;
struct PoolList;
struct StreamMutex;
struct TaggedPoolList;

#include "game/pool_list.h"
#include "game/runtime.h"
#include "game/stream_manager.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
struct ManagerLock : StreamMutex {
    ManagerLock() { critical.state = 0; owner = 0; depth = ~0u; }
};
#endif

#ifdef __cplusplus
struct TaggedPoolList {
    TaggedPoolList();
    unsigned tag;
    PoolList list;
};
#endif

#ifdef __cplusplus
struct LockedListOwner {
    LockedListOwner();
    const LockedListVTable *vtable;
    ManagerLock lock;
    TaggedPoolList lists;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern const struct LockedListVTable data_00224ae8;
#ifdef __cplusplus
}
#endif

#endif
