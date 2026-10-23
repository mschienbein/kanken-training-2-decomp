#include "game/display_initializer.h"

/* [INFERENCE] Natural slot-shaped setter, not a recovered original identity.
 * Its index is caller-evidenced even though this field is shared by all slots. */
void w16_set_screen(struct BoundDisplayObject *object, int index, int screen)
{
    object->screen = screen;
}
