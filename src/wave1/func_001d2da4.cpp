#include "game/resource.h"
#include "game/runtime.h"

extern "C" void func_001d2da4(ResourceOwner *owner)
{
    if ((owner->resource_bits & ~1u) != 0) {
        if ((owner->resource_bits & 1u) != 0)
            func_0019f568();
        func_0019d5c4(reinterpret_cast<Resource *>(owner->resource_bits & ~1u));
        owner->resource_bits = 0;
    }
}
