#include "game/functions.h"


struct ResourceRegistry {
    unsigned char reserved00[0x10];
    struct Registry entries;
};

struct RegisteredResource {
    void *type;
    struct RegistryNode link;
    unsigned int registered;
};


void func_00130f34(struct ResourceRegistry *registry, struct RegisteredResource *resource)
{
    func_0018d868(&registry->entries, &resource->link);
    resource->registered = 0;
}
