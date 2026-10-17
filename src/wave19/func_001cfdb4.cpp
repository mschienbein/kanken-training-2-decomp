struct W19Child {
    unsigned char reserved00[0x0e];
    bool disabled;
};

struct W19ChildOwner;
struct W19ChildOwnerVTable {
    const void *reserved00[6];
    W19Child *(*find_child)(W19ChildOwner *, int);
};

struct W19ChildOwner {
    const W19ChildOwnerVTable *vtable;
};

/* [INFERENCE] Pointer-only entry: the second input selects the child; the
 * boolean input is stored inverted. No return-value use is evidenced. */
extern "C" void func_001cfdb4(W19ChildOwner *owner, int index, bool enabled)
{
    W19Child *child = owner->vtable->find_child(owner, index);
    if (child != 0)
        child->disabled = !enabled;
}
