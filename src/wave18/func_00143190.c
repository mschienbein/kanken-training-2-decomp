struct ResourceHeader { unsigned kind; };
struct ResourceRef { struct ResourceHeader *data; };
struct ResourceCallbacks {
    void (*internal)(struct ResourceHeader *resource);
    void (*external)(struct ResourceHeader *resource);
};
extern struct ResourceCallbacks data_002437e0;

void func_00143190(struct ResourceRef *reference)
{
    struct ResourceHeader *resource = reference->data;
    if (resource->kind == 0x80000000u)
        data_002437e0.internal(resource);
    else if (resource->kind == 0x40000000u)
        data_002437e0.external(resource);
}
