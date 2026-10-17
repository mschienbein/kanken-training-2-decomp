#include "game/sound_track.h"
int func_001ada74(const struct SelectionOwner *owner)
{
    return owner->selection == -1 ? -1 : owner->identifier;
}
