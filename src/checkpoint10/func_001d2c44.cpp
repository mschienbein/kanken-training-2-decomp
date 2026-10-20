#include <preinclude.h>
#include "game/sound_runtime.h"
#include <cstddef>
#include <nw/ut/ut_LinkList.h>

/* [INFERENCE] Shared target-version sound/player prefixes preserve the actual
 * +0x100/+0x108 list subobjects. The erased detach boundary does not establish
 * a historical source call identity; its full typed inputs remain preserved. */

static inline void remove_priority(SoundPlayer *player, SoundInstance *sound)
{
    PrioritySoundList *list = &player->priorityList;
    list->Erase(sound);
}

static inline void remove_play(SoundPlayer *player, SoundInstance *sound)
{
    PlaySoundList *list = &player->playList;
    list->Erase(sound);
    func_001d2c78(sound, player);
}

extern "C" void func_001d2c44(SoundPlayer *player, SoundInstance *sound)
{
    remove_priority(player, sound);
    remove_play(player, sound);
}
