struct FlaggedResource {
    const void *vtable;
    unsigned char reserved04[0x0c];
    unsigned flags;
};

extern "C" unsigned func_001f4cac(const FlaggedResource *resource)
{
    return resource->flags;
}
