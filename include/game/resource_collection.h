#ifndef GAME_RESOURCE_COLLECTION_H
#define GAME_RESOURCE_COLLECTION_H
#include "game/runtime.h"
#include "game/shader.h"

/* [INFERENCE] Descriptive collection/subscriber roles. The virtual call keeps
 * the receiver and explicit keep-storage argument witnessed by the teardown. */
struct ResourceCollection;
class ResourceSubscriber {
public:
    virtual void reserved00();
    virtual void reserved04();
    virtual void reserved08();
    virtual void release(unsigned short identifier, ResourceCollection *collection);
};
struct ResourceUseLink {
    ResourceUseLink *next;
    ResourceUseLink *previous;
    ResourceSubscriber *subscriber;
    unsigned short identifier;
    unsigned char active;
    unsigned char reserved0f;
};
struct ResourceCollectionVTable {
    void (*reserved00)();
    void (*reserved04)();
    void (*reserved08)();
    void (*clear)(ResourceCollection *collection, int keep_storage);
};
struct ResourceCollection {
    const ResourceCollectionVTable *vtable;
    unsigned reserved04;
    ResourceUseLink users;
    unsigned char reserved18[0x1c];
    void *allocation;
    void clear() { vtable->clear(this, 0); }
};
extern "C" {
extern const ResourceCollectionVTable data_00224480;
void func_001cfe24(ResourceCollection *collection);
}
#endif
