#include "game/graphics_address.h"

void func_001223c0(struct GpuBufferDescriptor *descriptor, unsigned flags,
                   void *owner, const void *address, unsigned format,
                   unsigned height, unsigned width)
{
    descriptor->flags = flags;
    descriptor->owner = owner;
    if (address)
        descriptor->address_units = func_0018f0ac(address) >> 3;
    else
        descriptor->address_units = 0;
    descriptor->format = format;
    descriptor->height = height;
    descriptor->width = width;
}
