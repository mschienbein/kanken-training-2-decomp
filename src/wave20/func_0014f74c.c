struct TextureResource;
struct GraphicsResourceLink {
    struct TextureResource *resource;
    unsigned kind;
    unsigned identifier;
    struct GraphicsResourceLink *next;
};
struct GraphicsResourceTable {
    struct TextureResource *defaults[2];
    unsigned reserved08[2];
    struct GraphicsResourceLink *buckets[512];
};
extern struct GraphicsResourceTable *data_00242a0c;

struct TextureResource *func_0014f74c(unsigned identifier)
{
    struct GraphicsResourceLink *link;
    struct TextureResource *resource = 0;
    if (identifier == 0)
        return data_00242a0c->defaults[1];
    link = data_00242a0c->buckets[identifier & 511];
    while (link != 0) {
        if (link->identifier == identifier)
            break;
        link = link->next;
    }
    if (link != 0)
        resource = link->resource;
    if (link != 0 && resource != 0 && link->kind == 1)
        return resource;
    return 0;
}
