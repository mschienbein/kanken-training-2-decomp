#include "game/sound_format.h"

/* INTEGRATOR: extend SoundArchiveAddonVTable in include/game/archive.h with
 * the sound-info slot at 0x18; the frozen header currently leaves it padded. */
struct SoundArchiveAddonSoundVTable {
    unsigned char reserved00[0x18];
    bool (*read_sound)(SoundArchiveAddon *, unsigned, SoundBasicInfo *);
};

/* INTEGRATOR: move to include/game/archive.h. The bool return differs from
 * src/wave9/func_0013923c.c's int: that body returns only 0/1, while this
 * NintendoWare ReadSoundInfo caller consumes the boolean result directly. */
extern "C" {
bool func_0013923c(const SoundArchiveReader *reader, unsigned id, SoundBasicInfo *out);
bool func_001858dc(const SoundArchive *archive, unsigned id, SoundBasicInfo *out);
}

static bool read_addon_sound(SoundArchiveAddon *addon, unsigned id, SoundBasicInfo *out)
{
    if (!addon->enabled)
        return false;
    const SoundArchiveAddonSoundVTable *vtable =
        reinterpret_cast<const SoundArchiveAddonSoundVTable *>(addon->vtable);
    return vtable->read_sound(addon, id, out);
}

bool func_001858dc(const SoundArchive *archive, unsigned id, SoundBasicInfo *out)
{
    bool result = func_0013923c(archive->reader, id, out);
    SoundArchiveAddon *addon = archive->addon;
    if (addon != 0)
        result |= read_addon_sound(addon, id, out);
    return result;
}
