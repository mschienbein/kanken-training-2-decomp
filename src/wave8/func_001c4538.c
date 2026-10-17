struct ResourceCursor {
    unsigned resource_bits;
    unsigned position_low;
    unsigned position_high;
    unsigned length_low;
    unsigned length_high;
};
struct PositionedResourceOwner {
    const void *vtable;
    struct ResourceCursor cursor;
};

extern unsigned func_00180578(struct ResourceCursor *cursor, long long position);

unsigned func_001c4538(struct PositionedResourceOwner *owner, long long position)
{
    return func_00180578(&owner->cursor, position);
}
