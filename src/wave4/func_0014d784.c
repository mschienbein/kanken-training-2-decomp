#include "game/functions.h"

struct RegisteredObject {
    unsigned char reserved[0x88];
    struct RegistryEntry entry;
};

struct RegisteredObject *func_0014d784(struct RegisteredObject *object)
{
    func_0017e7bc(func_0017e7c4(), &object->entry);
    return object;
}
