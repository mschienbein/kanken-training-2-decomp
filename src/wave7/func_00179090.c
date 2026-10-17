#include "game/text.h"

/* Returned text is consumed as 16-bit code units by func_001771fc / func_0015ddec. */
const unsigned short *func_00179090(const struct MenuItem *item)
{
    return item->link_count > 0 ? item->link : 0;
}
