class Resource;

struct ResourceOwner {
    const void *vtable;
    unsigned resource_bits;
};

extern "C" void func_0019f568(void);
extern "C" void func_0019d5c4(Resource *resource);

extern "C" void func_001d2da4(ResourceOwner *owner)
{
    if ((owner->resource_bits & ~1u) != 0) {
        if ((owner->resource_bits & 1u) != 0)
            func_0019f568();
        func_0019d5c4(reinterpret_cast<Resource *>(owner->resource_bits & ~1u));
        owner->resource_bits = 0;
    }
}
