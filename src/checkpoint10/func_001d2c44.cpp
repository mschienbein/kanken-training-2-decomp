#include <preinclude.h>
#include <cstddef>
#include <nw/ut/ut_LinkList.h>

/* [INFERENCE] NW4C sound-player code (middleware): SoundPlayer::detail_RemoveSound, i.e.
 * RemovePriorityList(sound) then RemoveSoundList(sound) with both helpers inlined. The game's
 * BasicSound layout is 0x24 bytes larger than the vendored NW4C header's, so the two list links
 * are modelled here with their image offsets instead of the header class. */
struct PlayerSound;
struct SoundPlayerState;

/* INTEGRATOR: move to include/game/sound_runtime.h */
extern "C" {
/* [INFERENCE] callee identity: BasicSound::DetachSoundPlayer(SoundPlayer *); the 12-byte callee
 * only clears the sound's player slot and is reached by a tail branch that armlink turned into a NOP. */
void func_001d2c78(PlayerSound *sound, SoundPlayerState *player);
void func_001d2c44(SoundPlayerState *player, PlayerSound *sound);
}

struct PlayerSound {
    unsigned char reserved00[0x100];
    nw::ut::LinkListNode playLink;
    nw::ut::LinkListNode priorityLink;
};

typedef nw::ut::LinkList<PlayerSound, offsetof(PlayerSound, playLink)> PlaySoundList;
typedef nw::ut::LinkList<PlayerSound, offsetof(PlayerSound, priorityLink)> PrioritySoundList;

struct SoundPlayerState {
    PlaySoundList playList;
    PrioritySoundList priorityList;
};

static inline void remove_priority(SoundPlayerState *player, PlayerSound *sound)
{
    PrioritySoundList *list = &player->priorityList;
    list->Erase(sound);
}

static inline void remove_play(SoundPlayerState *player, PlayerSound *sound)
{
    PlaySoundList *list = &player->playList;
    list->Erase(sound);
    func_001d2c78(sound, player);
}

extern "C" void func_001d2c44(SoundPlayerState *player, PlayerSound *sound)
{
    remove_priority(player, sound);
    remove_play(player, sound);
}
