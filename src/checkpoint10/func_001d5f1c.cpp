#include "game/audio_voice.h"
#include "game/object_manager.h"
#include "game/command_group.h"
#include "game/sequence.h"

#include "game/wave_player.h"

static void append_item(PoolList *list, PoolLink *position, PendingTask68 *item)
{
    func_00191154(list, position, &item->link);
}

extern "C" void func_001d5f1c(W15iWavePlayer *player)
{
    player->stopping = 1;
    if (player->managed) {
        func_0017e634(func_00185d1c(), &player->managed_entry);
        player->managed = 0;
    }
    if (player->flag0d)
        player->flag0d = 0;
    if (player->registered) {
        func_0017e7bc(func_0017e7c4(), &player->entry);
        if (player->voice) {
            if (player->voice->flag139) {
                func_001d59c0(player);
                func_0017daf8(player->voice);
                if (!player->voice)
                    goto voice_done;
            }
            func_0017dae0(player->voice);
            player->voice = 0;
        }
voice_done:
        player->registered = 0;
    }
    if (player->event_active) {
        func_0019ea64(player);
        player->event_active = 0;
    }
    PendingTask68 *item = player->item;
    if (item) {
        W15iSoundPool *pool = player->pool;
        if (!func_0019ec14(&item->completion)) {
            append_item(&pool->available, &pool->available.sentinel, item);
        } else {
            if (item->queued)
                func_0016a0d0(item->context, item->queued);
            append_item(&pool->playing, &pool->playing.sentinel, item);
        }
        player->item = 0;
    }
}
