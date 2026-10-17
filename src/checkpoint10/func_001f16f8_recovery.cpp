#include "game/archive.h"

struct SoundArchiveWaveInfo;
/* INTEGRATOR: extend SoundArchiveAddonVTable in include/game/archive.h with
 * the wave-archive-info slot at 0x28, currently part of its reserved24 gap. */
struct SoundArchiveAddonWaveVTable {
    unsigned char reserved00[0x28];
    bool (*read_wave_archive)(SoundArchiveAddon *, unsigned, SoundArchiveWaveInfo *);
};

/* INTEGRATOR: move to include/game/archive.h */
extern "C" {
bool func_001f2b04(const SoundArchiveReader *reader, unsigned id, SoundArchiveWaveInfo *out);
bool func_001f16f8(const SoundArchive *archive, unsigned id, SoundArchiveWaveInfo *out);
}

static bool read_addon_wave(SoundArchiveAddon *addon, unsigned id, SoundArchiveWaveInfo *out)
{
    if (!addon->enabled)
        return false;
    const SoundArchiveAddonWaveVTable *vtable =
        reinterpret_cast<const SoundArchiveAddonWaveVTable *>(addon->vtable);
    return vtable->read_wave_archive(addon, id, out);
}

bool func_001f16f8(const SoundArchive *archive, unsigned id, SoundArchiveWaveInfo *out)
{
    bool result = func_001f2b04(archive->reader, id, out);
    SoundArchiveAddon *addon = archive->addon;
    if (addon != 0)
        result |= read_addon_wave(addon, id, out);
    return result;
}
