/* INTEGRATOR: differs from include/game/item_pages.h: ItemPage also has
 * writable halfword-row pointers at 0x18 and writable flags at 0x1c. */
struct ItemPage {
    unsigned char reserved00[0xc];
    int count;
    unsigned char reserved10[8];
    unsigned short **values;
    unsigned char *flags;
};

void func_0015f724(struct ItemPage *table, int index, int column,
                   int value)
{
    unsigned short *row = table->values[index];
    if (row != 0)
        row[column] = value;
}
