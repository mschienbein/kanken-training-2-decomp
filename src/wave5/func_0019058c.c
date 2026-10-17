#include "game/service_state.h"
struct ServiceObject {
    unsigned handle;
};

unsigned func_0019058c(const struct ServiceObject *object)
{
    void *memory;
    unsigned result = func_0018fa44(func_0018fcb4(), &memory, object->handle);
    switch (result >> 31) {
    case 0:
        return 0;
    default:
        return result;
    }
}
