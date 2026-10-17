struct W15fFlagChild;
struct W15fFlagOwner {
    unsigned char reserved00[7];
    unsigned char flags;
    unsigned char reserved08[0x60];
    struct W15fFlagChild *child;
};

extern void func_00152610(struct W15fFlagChild *child, unsigned char flags);

void func_001525f0(struct W15fFlagOwner *owner, int enabled)
{
    if (enabled)
        owner->flags |= 1;
    else
        owner->flags &= ~1;
    func_00152610(owner->child, owner->flags);
}
