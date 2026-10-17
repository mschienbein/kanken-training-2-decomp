#include <stddef.h>

#include "game/sound_runtime.h"

int func_001d007c(const struct SoundFileGroup *player, int requested_priority)
{
    struct SoundInstance *sound;
    if (player->requested == 0)
        goto reject;
    if (player->requested > (int)player->playing.count)
        goto accept;
    sound = (struct SoundInstance *)
        ((char *)player->pending.sentinel.next - offsetof(struct SoundInstance, player_link));
    if (sound == 0)
        goto reject;
    if (player->active) {
        int priority = __usat(sound->priority + sound->priority_offset, 7);
        if (requested_priority <= priority)
            goto reject;
        goto accept;
    }
    if ((int)__usat(sound->priority + sound->priority_offset, 7) <= requested_priority)
        goto accept;
reject:
    return 0;
accept:
    return 1;
}
