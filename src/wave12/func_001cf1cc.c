#include "game/command_group.h"
#include "game/resource_font.h"
/* [INFERENCE] Packed four-channel values: func_001cf1dc addresses the same
 * storage as a byte array using the color index and channel. */


void func_001cf1cc(struct LayoutColors140 *self, unsigned index,
                   const struct LayoutColor *color)
{
    self->colors[index] = *color;
}
