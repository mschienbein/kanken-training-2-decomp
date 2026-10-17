#include <string.h>

struct ResourceAllocator;
struct ResourceAllocatorVTable {
    unsigned char reserved00[8];
    void *(*allocate)(ResourceAllocator *allocator, unsigned size);
};

struct ResourceAllocator {
    const ResourceAllocatorVTable *vtable;

};

struct ResourceHeader {
    unsigned word00;
    unsigned word04;
    ResourceAllocator *allocator;
};

struct ResourceDescription {
    ResourceHeader header;
    const void *data;
    unsigned size;
};

struct ResourceDestination {
    unsigned char reserved00[0x1c];
    ResourceHeader header;
    void *allocation;
    unsigned size;
};

extern "C" void func_0016a830(ResourceDestination *destination,
                             const ResourceDescription *description)
{
    ResourceAllocator *allocator = description->header.allocator;
    void *allocation = allocator->vtable->allocate(allocator, description->size);
    if (allocation) {
        memcpy(allocation, description->data, description->size);
        ResourceHeader header = description->header;
        unsigned size = description->size;
        destination->header = header;
        destination->allocation = allocation;
        destination->size = size;
    }
}
