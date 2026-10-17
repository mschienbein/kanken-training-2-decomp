#include "game/property_descriptor.h"

FloatPropertyDescriptor *func_00164ddc(FloatPropertyDescriptor *object,
                                      int category, int identifier,
                                      const char *name, float x, float y, float z,
                                      int count, ...)
{
    va_list arguments;
    object = reinterpret_cast<FloatPropertyDescriptor *>(
        func_0016e06c(&object->base));
    object->base.vtable = &data_00223678;
    func_0015f5a4(&object->base, category, identifier, name);
    va_start(arguments, count);
    func_001b7d50(&object->base, count, arguments);
    object->x = x;
    object->y = y;
    object->z = z;
    va_end(arguments);
    return object;
}
