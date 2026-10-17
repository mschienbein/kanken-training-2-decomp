#include "game/sound_track.h"
unsigned char *func_001cfd40(const struct ModeTableState *state)
{
    unsigned char *table = state->table;
    return table + ((unsigned)state->mode << 30 >> 25);
}
