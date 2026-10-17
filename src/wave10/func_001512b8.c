struct Child1512b8;

struct S1512b8 {
    unsigned char reserved[0x90];
    struct Child1512b8 *children[16];
};

struct Child1512b8 *func_001512b8(const struct S1512b8 *object, int index)
{
    /* ORIGINAL BUG: negative indices are not rejected. */
    if (index > 15)
        return 0;
    return object->children[index];
}
