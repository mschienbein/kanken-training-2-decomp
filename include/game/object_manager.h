#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "game/stream_manager.h"


struct ObjectManager {
    unsigned char reserved00[0x1c4];
    struct StreamMutex lock;
    unsigned char reserved1d0[0x0c];
    struct PoolList active;                 /* 0x1dc */
};

#ifdef __cplusplus
extern "C" {
#endif
extern void func_00195ec4(struct StreamMutex *lock);
extern void func_0017e634(struct ObjectManager *manager, struct RegistryEntry *entry);
extern struct PoolLink *func_0017e640(struct ObjectManager *manager,
                                     struct RegistryEntry *entry);
#ifdef __cplusplus
}
#endif

#endif
