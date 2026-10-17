#include "game/audio_voice.h"
static __forceinline unsigned char *get_address(const struct PackedPointer001f15fc *state)
{
    return state->address;
}

void *func_001f15fc(const struct PackedPointer001f15fc *state)
{
    unsigned char *address = get_address(state);
    /* The low two selector bits select a 32-byte section. */
    return address + (((unsigned)state->selector << 30) >> 25);
}
