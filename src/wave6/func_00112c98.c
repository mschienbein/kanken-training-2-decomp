struct ServiceResetState { unsigned int generation; };
struct ServiceEntry {
    unsigned int value;
    unsigned char kind, variant;
    unsigned short attributes;
};
struct ServiceList {
    struct ServiceEntry *entries;
    unsigned int capacity;
    unsigned int count;
    unsigned int remainder[36];
};
extern struct ServiceList bss_004b47d0;
extern void func_00192720(void *destination, const void *source, unsigned size);

void func_00112c98(struct ServiceResetState *state)
{
    struct ServiceList list;
    ++state->generation;
    /* ORIGINAL BUG: the reset copies an uninitialized local list into global state. */
    func_00192720(&bss_004b47d0, &list, sizeof(list));
}
