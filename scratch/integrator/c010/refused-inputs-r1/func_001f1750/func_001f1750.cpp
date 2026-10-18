#include "game/archive.h"

struct ArchiveStreamSoundInfo;
struct ArchiveStreamExtensionInfo;
struct SoundArchiveAddonVTable {
    unsigned char reserved00[0x40];
    unsigned (*read_stream)(SoundArchiveAddon *addon, unsigned sound_id,
                            ArchiveStreamSoundInfo *output);
    unsigned (*read_extension)(SoundArchiveAddon *addon, unsigned sound_id,
                               ArchiveStreamExtensionInfo *output);
};
struct SoundArchiveAddon {
    const SoundArchiveAddonVTable *vtable;
    unsigned char enabled;
};

/* INTEGRATOR: move to include/game/archive.h */
extern "C" {
unsigned func_001f2bac(const SoundArchiveReader *reader, unsigned sound_id,
                       ArchiveStreamSoundInfo *output);
unsigned func_001f1750(const SoundArchive *archive, unsigned sound_id,
                       ArchiveStreamSoundInfo *output);
}

static __forceinline unsigned read_addon_stream(SoundArchiveAddon *addon, unsigned sound_id,
                                                 ArchiveStreamSoundInfo *output)
{
    if (addon->enabled)
        return addon->vtable->read_stream(addon, sound_id, output);
    return 0;
}

unsigned func_001f1750(const SoundArchive *archive, unsigned sound_id,
                       ArchiveStreamSoundInfo *output)
{
    unsigned result = func_001f2bac(archive->reader, sound_id, output);
    SoundArchiveAddon *addon = archive->addon;
    if (addon != 0)
        result |= read_addon_stream(addon, sound_id, output);
    return result;
}
