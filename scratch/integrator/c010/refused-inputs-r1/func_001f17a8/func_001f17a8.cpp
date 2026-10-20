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
unsigned func_001f2e3c(const SoundArchiveReader *reader, unsigned sound_id,
                       ArchiveStreamExtensionInfo *output);
unsigned func_001f17a8(const SoundArchive *archive, unsigned sound_id,
                       ArchiveStreamExtensionInfo *output);
}

static __forceinline unsigned read_addon_extension(SoundArchiveAddon *addon, unsigned sound_id,
                                                    ArchiveStreamExtensionInfo *output)
{
    if (addon->enabled)
        return addon->vtable->read_extension(addon, sound_id, output);
    return 0;
}

unsigned func_001f17a8(const SoundArchive *archive, unsigned sound_id,
                       ArchiveStreamExtensionInfo *output)
{
    unsigned result = func_001f2e3c(archive->reader, sound_id, output);
    SoundArchiveAddon *addon = archive->addon;
    if (addon != 0)
        result |= read_addon_extension(addon, sound_id, output);
    return result;
}
