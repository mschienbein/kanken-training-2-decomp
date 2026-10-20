struct W17ResourceFlags {
    void *resource;
    unsigned char flags;
};

struct W17ResourceObject {
    unsigned int reserved00[10];
    struct W17ResourceFlags *entry;
};

int func_001b51ec(const struct W17ResourceObject *object)
{
    unsigned int result;
    if (!object->entry || !object->entry->resource)
        return 0;
    result = object->entry->flags & 8;
    if (result)
        result = 1;
    return result;
}
