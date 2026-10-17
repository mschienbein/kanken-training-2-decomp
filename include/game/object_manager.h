#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

struct PoolLink;
struct PoolList;
struct RegistryEntry;
struct StreamMutex;


#include "game/stream_manager.h"


struct ObjectManager {
    unsigned char reserved00[0x1b8];
    unsigned field1b8, field1bc, field1c0;
    struct StreamMutex lock;
    unsigned char reserved1d0[0x0c];
    struct PoolList active;                 /* 0x1dc */
    unsigned char reserved1e8[0x0c];
    unsigned char running;
    unsigned char reserved1f5[2];
    unsigned char stopping;
};

struct OperationSettings {
    unsigned value;
    unsigned char mode;
    unsigned char reserved05[3];
    unsigned duration;
};
struct OperationTransform {
    float values[6];
};
struct OperationKey {
    unsigned first;
    unsigned second;
    unsigned char mode;
};
struct Operation {
    unsigned char reserved00[0x0c];
    unsigned char active;
    unsigned char running;
    unsigned char reserved0e;
    unsigned char stopping;
    unsigned char reserved10[0x34];
    struct RegistryEntry registry_entry;
    struct RegistryEntry manager_entry;
    unsigned char reserved5c[3];
    unsigned char key_mode;
    unsigned reserved60;
    unsigned key_first;
    unsigned key_second;
    struct OperationSettings settings;
    unsigned char reserved78[0x28];
    unsigned char state;
    unsigned char reserveda1;
    unsigned char linked;
    unsigned char reserveda3[9];
    struct OperationTransform transform;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_00150658(struct Operation *operation, const struct OperationSettings *settings,
                          const struct OperationKey *key);
extern void func_001f4e48(struct RegistryEntry *entry, unsigned first, unsigned last);
extern void func_00195ec4(struct StreamMutex *lock);
extern void func_0017e634(struct ObjectManager *manager, struct RegistryEntry *entry);
extern struct PoolLink *func_0017e640(struct ObjectManager *manager,
                                     struct RegistryEntry *entry);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
extern void func_001d5c10(struct Operation *operation);
#ifdef __cplusplus
}
#endif

#endif
