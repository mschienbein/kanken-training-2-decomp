#include "game/runtime.h"
#include "game/screen.h"
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
struct DisplayRegistrationProxy {
    unsigned char reserved00[8];
    struct RegistrationEntry *entry;
};
struct DisplayRegistrationBinding {
    unsigned char reserved00[0x28];
    struct DisplayRegistrationProxy *proxy;
    unsigned char reserved2c[8];
    struct RegistrationEntry *entry;
};
struct ResourceGroupObject;

/* INTEGRATOR: this tagged state refines DisplayObject's reserved bytes and
 * ResourceRegistrationSlot's reserved tail. Binding/object share the tagged
 * pointer at 0x94, enabled is at 0xa0, and the signed slot count is at 0xd0. */
struct RenderEntryState {
    struct DisplayObject base;
    union {
        struct ResourceGroupObject *object;
        struct DisplayRegistrationBinding *binding;
    };
    unsigned char flag98;
    unsigned char reserved99[3];
    int value9c;
    int enabled;
    float valuea4;
    int valuea8;
    int valueac;
    struct RenderEntryState *next;
    int valueb4;
    float depth;
    unsigned valuebc;
    unsigned char active;
    unsigned char reservedc1[3];
    int indexc4;
    int indexc8;
};
struct ResourceGroupObject {
    const void *const *vtable;
    struct PoolList entries;
    struct RegistrationOwner *registry;
    int value14;
    float width;
    float height;
    void *owned;
    void *allocation;
    unsigned char active;
    unsigned char reserved29[3];
    struct RenderEntryState *children;
};
struct ResourceRegistrationRecord {
    struct ResourceGroupObject *resource;
    struct RenderEntryState state;
    int registrations;
};
/* INTEGRATOR: move to include/game/scene_runtime.h; the RegistrationOwner,
 * RegistrationEntry and func_001822fc contracts exactly copy
 * src/wave6/func_001822fc.c. */
extern "C" {
void func_001822fc(struct RegistrationOwner *owner,
                   struct RegistrationEntry *entry);
}

static struct ResourceRegistrationRecord *registration_slot(int screen, int index)
{
    short group = -1;
    if (screen == 3)
        group = 0;
    else if (screen == 4)
        group = 1;
    else
        func_0019f568();
    /* ORIGINAL BUG: invalid screens index before the group table if fatal returns. */
    unsigned char *word = reinterpret_cast<unsigned char *>(&bss_0049cbd0);
    word += group * sizeof(struct ResourceRegistrationGroup);
    word += index * sizeof(struct ResourceRegistrationSlot);
    return reinterpret_cast<struct ResourceRegistrationRecord *>(word + 0x1c);
}

static void register_node(struct RegistrationOwner *owner,
                          struct RenderEntryState *node, int kind,
                          int screen, int index)
{
    if (kind == 5)
        func_001822fc(owner, node->binding->proxy->entry);
    else
        func_001822fc(owner, node->binding->entry);
    ++registration_slot(screen, index)->registrations;
}

void func_00196144(int screen, int index)
{
    struct ResourceGroupObject *resource =
        static_cast<struct ResourceGroupObject *>(func_00196cb8(screen, index));
    struct RenderEntryState *node;
    for (node = resource->children; node; node = node->next) {
        if (node->enabled) {
            switch (node->base.types[0]) {
            case 6:
                register_node(resource->registry, node, 6, screen, index);
                break;
            case 5:
                register_node(resource->registry, node, 5, screen, index);
                break;
            }
        }
    }
}

