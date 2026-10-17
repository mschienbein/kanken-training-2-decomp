struct SlotValue {
    void *value;
};

struct SlotTable {
    unsigned char reserved00[0x828];
    const struct SlotValue *slots[32];
};

extern const struct SlotTable *data_00242a0c;

void *func_0018cb5c(unsigned index)
{
    if (index < 32) {
        const struct SlotValue *slot = data_00242a0c->slots[index];
        if (slot)
            return slot->value;
    }
    return 0;
}
