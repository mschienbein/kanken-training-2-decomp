#include "game/region_heap.h"
#include "game/functions.h"
#include <stddef.h>

struct SynchronizationState {
    volatile unsigned state;
};
struct SynchronizationOwner {
    const void *vtable;
    struct SynchronizationState synchronization;
};
struct SceneEntry;
struct SceneEntryVtable {
    void (*reset)(struct SceneEntry *entry);
};
struct SceneEntry {
    union {
        const struct SceneEntryVtable *vtable;
        struct FreeLink free_link;
    } storage;
    unsigned char reserved04[8];
    struct SynchronizationState synchronization;
    unsigned char reserved10[4];
    unsigned char state;
    unsigned char reserved15[0x50 - 0x15];
    struct PoolLink link;
};
struct SceneRenderer;
struct SceneRendererVtable {
    unsigned char reserved00[0x38];
    void (*release)(struct SceneRenderer *renderer);
};
struct SceneRenderer {
    const struct SceneRendererVtable *vtable;
};
struct PolymorphicBase {
    const void *vtable;
};
struct SceneEngine {
    unsigned char reserved00[0x14];
    struct SceneRenderer *renderer;
    unsigned char reserved18[0x51 - 0x18];
    unsigned char inhibited;
    unsigned char reserved52[0x64 - 0x52];
    struct PolymorphicBase base;
    unsigned char reserved68[8];
    struct SynchronizationState synchronization;
    unsigned char reserved74[0x84 - 0x74];
    struct PoolList active;
    struct FreeLink *free_list;
    struct SceneEntry entries[32];
};

/* INTEGRATOR: move to include/game/scene_lifecycle.h */
extern "C" {
void func_0017f4f4(struct FreeLink **head, struct FreeLink *node);
void func_001d6ab8(struct SceneEngine *engine);
}

struct SceneIterator {
    struct PoolLink *node;
    SceneIterator(struct PoolLink *value) : node(value) {}
    struct SceneEntry *get() const
    {
        return (struct SceneEntry *)((char *)node - offsetof(struct SceneEntry, link));
    }
    SceneIterator operator++(int)
    {
        SceneIterator old = *this;
        node = node->next;
        return old;
    }
    bool operator!=(SceneIterator other) const
    {
        return node != other.node;
    }
};

static __forceinline SceneIterator endOf(struct PoolList *registry)
{
    SceneIterator result(&registry->sentinel);
    return result;
}

static __forceinline void recycleEntry(struct FreeLink **head, struct SceneEntry *entry)
{
    if (entry) {
        entry->storage.vtable->reset(entry);
        func_0017f4f4(head, &entry->storage.free_link);
    }
}
/* [INFERENCE] source shape: a by-value list handle whose erase() forwards to the shared list-erase routine
 * (NW4C LinkListImpl::erase); the temporary handle is what ARMCC 4.1 needs to order the call arguments. */
struct ListHandle {
    struct PoolList *list;
    ListHandle(struct PoolList *value) : list(value) {}
    struct PoolLink *erase(struct PoolLink *node) { return func_0018d868(list, node); }
};

void func_001d6ab8(struct SceneEngine *engine)
{
    SceneIterator iterator(engine->active.sentinel.next);
    if (iterator != endOf(&engine->active)) {
        do {
            struct SceneEntry *entry = iterator.get();
            iterator.node = iterator.node->next;
            if (entry->state != 3)
                break;
            ListHandle(&engine->active).erase(&entry->link);
            recycleEntry(&engine->free_list, entry);
        } while (iterator != endOf(&engine->active));
    }
}
