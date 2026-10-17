#include "game/archive.h"

const struct SoundPlayerInfo *func_0014c2a4(const struct SoundArchiveInfo *info)
{
    return (const struct SoundPlayerInfo *)((const unsigned char *)info + info->player_info_offset);
}
