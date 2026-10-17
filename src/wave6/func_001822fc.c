#include "game/pool_list.h"

struct RegistrationOwner {
    unsigned char reserved00[0x10];
    struct PoolList active;
};

struct RegistrationEntry {
    const void *vtable;
    struct PoolLink link;
    struct RegistrationOwner *owner;
};


static __forceinline struct PoolLink *entryLink(struct RegistrationEntry *entry)
{
    return &entry->link;
}

static __forceinline void setOwner(struct RegistrationOwner *owner, struct RegistrationEntry *entry)
{
    entry->owner = owner;
}

void func_001822fc(struct RegistrationOwner *owner, struct RegistrationEntry *entry)
{
    struct PoolLink *position = &owner->active.sentinel;
    struct PoolList *list = &owner->active;
    func_00191154(list, position, entryLink(entry));
    setOwner(owner, entry);
}
