#include "game/text.h"

/* [INFERENCE] Tiny item accessors are possible armlink-removed definitions;
 * neither helper identity nor original spelling is recovered. */
int r41279_item_identifier(const struct MenuItem *item)
{
    return (int)item->text.reserved00[3];
}

int r41279_item_category(const struct MenuItem *item)
{
    return (signed char)item->text.reserved10;
}
