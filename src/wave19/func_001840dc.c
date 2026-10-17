#include "game/functions.h"
struct BinaryResourceHeader {
    int type;
};
struct BinaryResourceReference {
    struct BinaryResourceHeader *resource;
};
struct ResourceDispatchHooks {
    void (*texture)(void *reference);
    void (*shader)(void *reference);
    void (*material)(void *reference);
    void (*model)(void *reference);
    void (*texture_cube)(void *reference);
};
extern struct ResourceDispatchHooks data_00243510;

void func_001840dc(struct BinaryResourceReference *reference)
{
    switch (reference->resource->type) {
    case 0x20000011:
        data_00243510.texture(reference->resource);
        break;
    case 0x20000009:
        data_00243510.shader(reference->resource);
        break;
    case 0x20000004:
        data_00243510.material(reference->resource);
        break;
    case 0x20000002:
        data_00243510.model(reference->resource);
        break;
    case 0x20000021:
        data_00243510.texture_cube(reference->resource);
        break;
    }
}
