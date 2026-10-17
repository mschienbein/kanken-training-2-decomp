#include "game/resource_instance.h"
#include "game/layout_binding.h"
#include "game/pool_list.h"

struct LayoutBindingAllocator;
struct LayoutBindingAllocatorVTable {
    unsigned char reserved00[8];
    void *(*allocate)(LayoutBindingAllocator *allocator, unsigned size, int alignment);
};
struct LayoutBindingAllocator {
    const LayoutBindingAllocatorVTable *vtable;
};
struct LayoutBindingManager {
    unsigned reserved00;
    PoolList bindings;
};

/* INTEGRATOR: move to include/game/layout_binding.h */
extern "C" {


LayoutBinding *func_001ce940(LayoutBindingManager *manager);
}

static void *allocate_binding(LayoutBindingAllocator *allocator, unsigned size, int alignment)
{
    return allocator->vtable->allocate(allocator, size, alignment);
}

static void insert_binding(PoolList *list, PoolLink *position, LayoutBinding *binding)
{
    func_00191154(list, position, &binding->link);
}

extern "C" LayoutBinding *func_001ce940(LayoutBindingManager *manager)
{
    void *storage =
        allocate_binding(data_00242f84, sizeof(LayoutBinding), 4);
    LayoutBinding *binding;
    if (storage)
        binding = func_001cce4c(static_cast<LayoutBinding *>(storage));
    else
        binding = 0;
    if (binding)
        insert_binding(&manager->bindings, &manager->bindings.sentinel, binding);
    return binding;
}
