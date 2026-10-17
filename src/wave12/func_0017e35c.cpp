#include "game/functions.h"
#include "game/object_manager.h"
#include "game/sequence.h"
#include "game/archive.h"
#include "game/audio_voice.h"




static AnimationTrack *getTrack(SequencePlayer *player, int index)
{
    if (index > 15)
        return 0;
    return player->tracks[index];
}

void func_0017e35c(SequencePlayer *player)
{
    if (player->registered) {
        func_0017e634(func_00185d1c(), &player->manager_entry);
        player->registered = 0;
    }
    if (player->paused)
        player->paused = 0;
    for (int i = 0; i < 16; ++i) {
        if (getTrack(player, i)) {
            func_0017deec(getTrack(player, i));
            player->allocator->vtable->release(player->allocator, getTrack(player, i));
            player->tracks[i] = 0;
        }
    }
}
