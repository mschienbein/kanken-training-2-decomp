#include "game/sound_task.h"

/* [INFERENCE] Target-version player lifetime from its construction, original
 * cleanup fields, stop receiver and pointer-preserving return. */
static void install_player_type(SoundStreamPlayer *player,
                                const SoundStreamPlayerTypeTables *tables)
{
    player->control_vtable = tables->control;
    player->base.vtable = tables;
    player->parameter_vtable = tables->parameter;
}

SoundStreamPlayer::~SoundStreamPlayer()
{
    install_player_type(this, &data_00224a00);
    func_001d8d7c(this);
    base.vtable = data_00224940;
}
