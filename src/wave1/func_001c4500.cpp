class Resource;

extern "C" void func_0019f568(void);
extern "C" void func_0019d5c4(Resource *resource);

struct ResourceSlot {
    unsigned bits;
    ~ResourceSlot()
    {
        if ((bits & ~1u) != 0) {
            if ((bits & 1u) != 0)
                func_0019f568();
            func_0019d5c4(reinterpret_cast<Resource *>(bits & ~1u));
            bits = 0;
        }
    }
};

struct DeletingResourceOwner {
    const void *vtable;
    ResourceSlot resource;
};

extern "C" DeletingResourceOwner *func_001c4500(DeletingResourceOwner *owner, int)
{
    ResourceSlot *slot = &owner->resource;
    slot->~ResourceSlot();
    return reinterpret_cast<DeletingResourceOwner *>(reinterpret_cast<char *>(slot) - 4);
}
