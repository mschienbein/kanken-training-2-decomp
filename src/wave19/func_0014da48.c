/* [INFERENCE] Field roles are unknown; widths follow the resource-record copy. */
struct W19ResourceEntry {
    unsigned word00, word04, word08, word0c;
    unsigned short half10, half12;
    unsigned char byte14, byte15;
    unsigned short half16;
    unsigned word18, word1c;
};

struct W19ResourceTable {
    unsigned char reserved00[6];
    unsigned short count;
    unsigned char reserved08[0x18];
    struct W19ResourceEntry entries[];
};

int func_0014da48(struct W19ResourceEntry *destination,
                  const struct W19ResourceTable *table, int index)
{
    const struct W19ResourceEntry *source;
    if (table == 0 || destination == 0 || index < 0 || index >= table->count)
        return 0;
    source = &table->entries[index];
    destination->word00 = source->word00;
    destination->word04 = source->word04;
    destination->word08 = source->word08;
    destination->word0c = source->word0c;
    destination->half10 = source->half10;
    destination->half12 = source->half12;
    destination->byte14 = source->byte14;
    destination->byte15 = source->byte15;
    destination->half16 = source->half16;
    destination->word18 = source->word18;
    destination->word1c = source->word1c;
    return 1;
}
