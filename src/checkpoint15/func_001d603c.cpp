#include "game/wave_player.h"

static void set_player_type(W15iWavePlayer *player, const void *table)
{
    player->vtable = table;
}

static void reset_player_types(W15iWavePlayer *player, const WavePlayerType *type)
{
    player->entry.type = type->registry;
    player->managed_entry.type = type->manager;
    set_player_type(player, type->primary);
}

extern "C" W15iWavePlayer *func_001d603c(W15iWavePlayer *player)
{
    reset_player_types(player, &data_002248e0);
    func_001d5f1c(player);
    set_player_type(player, data_00224940);
    return player;
}
