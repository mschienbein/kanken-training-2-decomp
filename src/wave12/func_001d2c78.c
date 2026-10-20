#include "game/sound_runtime.h"
/* [INFERENCE] Native and direct caller evidence keeps the unused player input.
 * The stored word is the actual sound's player pointer, not an integer state. */
void func_001d2c78(struct SoundInstance *object, struct SoundPlayer *player)
{
    object->player = 0;
}
