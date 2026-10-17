struct W17FlagWord {
    unsigned int flags;
};

struct W17FlagOwner {
    unsigned int reserved00[8];
    struct W17FlagWord *items[];
};

void func_00175a90(struct W17FlagOwner *owner, int index, int enabled)
{
    struct W17FlagWord *item = owner->items[index];
    if (item) {
        if (enabled)
            item->flags |= 8;
        else
            item->flags &= ~8u;
    }
}
