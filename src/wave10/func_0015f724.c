#include "game/item_pages.h"

void func_0015f724(struct ItemPage *table, int index, int column,
                   int value)
{
    unsigned short *row = table->values[index];
    if (row != 0)
        row[column] = value;
}
