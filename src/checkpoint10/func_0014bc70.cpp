#include <preinclude.h>
#include "game/stream_manager.h"
#include "nw/ut/ut_LinkList.h"
#include <stddef.h>

/* INTEGRATOR: differs from include/game/object_manager.h: the 12 bytes at 0x1d0 (reserved1d0)
 * are a registry nw::ut::LinkList; func_0018d868 and func_00191154 are bound to
 * LinkListImpl::erase(LinkListNode*) and ::insert(iterator, LinkListNode*) in config/names.json. */
struct ObjectManager {
    unsigned char reserved00[0x1b8];
    unsigned field1b8, field1bc, field1c0;
    StreamMutex lock;
    nw::ut::LinkList<RegistryEntry, offsetof(RegistryEntry, link)> registry;
    PoolList active;                        /* 0x1dc */
    unsigned char reserved1e8[0x0c];
    unsigned char running;
    unsigned char reserved1f5[2];
    unsigned char stopping;
};

/* [INFERENCE] Scoped lock; the original wrapper type is unrecovered. */
struct RegistryGuard {
    StreamMutex *mutex;
    RegistryGuard(StreamMutex *value) : mutex(value)
    {
        func_00192bd8(mutex);
    }
    ~RegistryGuard()
    {
        func_00195d74(mutex);
    }
};

extern "C" void func_0014bc70(ObjectManager *manager, RegistryEntry *entry)
{
    RegistryGuard guard(&manager->lock);
    manager->registry.push_back(entry);
}
