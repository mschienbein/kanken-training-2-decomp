#include "game/sound_track.h"
/* The caller identifies the selection bit to clear. */
void func_001adac8(struct SelectionOwner *owner, int selected_index)
{
    owner->flags &= ~(1u << selected_index);
}
