#include "game/font_resource.h"
struct ResourceArrayObject;
struct ResourceArrayVTable {
    unsigned char reserved00[0x68];
    int (*count)(ResourceArrayObject *object);
};
struct ResourceArrayEntry {
    unsigned int handle;
    unsigned int fields[4];
};
struct ResourceArrayObject {
    const ResourceArrayVTable *vtable;
    unsigned char reserved04[8];
    ResourceArrayEntry *entries;
};

extern "C" void func_00113e20(ResourceArrayObject *object)
{
    ResourceArrayEntry *entries = object->entries;
    int count = object->vtable->count(object);
    for (int index = 0; index < count; ++index) {
        unsigned int handle = entries[index].handle;
        if (handle) {
            func_0018f0dc(1, &handle);
            entries[index].handle = 0;
        }
    }
}
