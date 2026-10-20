struct AvailabilitySlot {
    void *object;
    unsigned char reserved04[0x30];
    int enabled;
    unsigned char reserved38[0x1c];
};
struct AvailabilityTable {
    unsigned char reserved00[0x18];
    struct AvailabilitySlot slots[];
};

int func_001b3400(const struct AvailabilityTable *table, int index)
{
    const struct AvailabilitySlot *slot = &table->slots[index];
    if (slot->enabled != 0) {
        if (slot->object == 0)
            return 1;
    }
    return 0;
}
