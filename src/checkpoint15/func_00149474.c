#include "game/object_manager.h"

/* Registered by func_00147124 as ServiceWorker.service, whose canonical
 * contract is void (*)(void *). The user callback's context role is [INFERENCE]. */
struct ManagerCallbackBinding {
    void (*callback)(void *context);
    void *context;
};

/* [INFERENCE] The source-level binding accessor is reconstructed from the
 * shared callback/context fields; its historical identity is not recovered. */
static struct ManagerCallbackBinding get_binding(const struct ObjectManager *owner)
{
    struct ManagerCallbackBinding binding;
    binding.context = owner->callback_context;
    binding.callback = owner->callback;
    return binding;
}
void func_00149474(void *manager)
{
    const struct ObjectManager *owner = manager;
    if (get_binding(owner).callback) {
        struct ManagerCallbackBinding binding = get_binding(owner);
        binding.callback(binding.context);
    }
}
