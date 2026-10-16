#include "game/resource.h"
#include "game/runtime.h"

struct Controller {
    char reserved00[0x34];
    ResourceOwner *owner;
    char reserved38[0x18];
    unsigned char active;
    unsigned char enabled;
};

extern "C" void func_001d3524(Controller *controller)
{
    if (controller->enabled != 0 && controller->active != 0) {
        ResourceOwner *owner = controller->owner;
        if ((owner->resource_bits & ~1u) != 0) {
            if ((owner->resource_bits & 1u) != 0)
                func_0019f568();
            func_0019d5c4(reinterpret_cast<Resource *>(owner->resource_bits & ~1u));
            owner->resource_bits = 0;
        }
        controller->active = 0;
    }
}
