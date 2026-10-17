struct CountedResource {
    unsigned char reserved00[6];
    signed char category;
    unsigned char reserved07[5];
    int count;
    unsigned char reserved10[4];
    signed char weight;
    unsigned char reserved15[7];
    const unsigned char *values;
};

struct CountedResourceList {
    unsigned char reserved00[0x10];
    int count;
    struct CountedResource **items;
};

extern int func_0015f5b0(const struct CountedResource *resource);

int func_001743d8(const struct CountedResourceList *list)
{
    int total = 0;
    int index;
    if (list->items) {
        for (index = 0; index < list->count; ++index) {
            struct CountedResource *resource = list->items[index];
            if (resource)
                total += func_0015f5b0(resource);
        }
    }
    return total;
}
