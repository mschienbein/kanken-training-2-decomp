struct RelativeCallbackOwner {
    unsigned char reserved00[8];
    int offset;
    unsigned char reserved0c[0x20];
    unsigned state;
};

extern void func_001840dc(void **object);

static void *ResolveRelative(int *relative)
{
    void *object = 0;
    if (*relative != 0)
        object = (unsigned char *)relative + *relative;
    return object;
}

void func_001c8af0(struct RelativeCallbackOwner *owner)
{
    void *object;
    owner->state = 0;
    object = ResolveRelative(&owner->offset);
    if (object != 0)
        func_001840dc(&object);
}
