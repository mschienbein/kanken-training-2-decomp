struct ResourceHeader { unsigned kind; };
struct ResourceRef { struct ResourceHeader *data; };
struct ResourceCallbacks {
    void (*internal)(struct ResourceHeader *resource);
    void (*external)(struct ResourceHeader *resource);
};
extern struct ResourceCallbacks data_002434e4;

void func_00182ae4(struct ResourceRef *reference)
{
    struct ResourceHeader *resource = reference->data;
    if (resource->kind == 0x80000000u)
        data_002434e4.internal(resource);
    else if (resource->kind == 0x40000000u)
        data_002434e4.external(resource);
}
