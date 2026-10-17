#include <preinclude.h>
#include "game/sound_loader.h"
#include <nw/snd/snd_SoundArchive.h>
#include <nw/snd/snd_WaveSoundFileReader.h>
#include "game/archive.h"

struct SoundLoaderState;
/* INTEGRATOR: move to include/game/sound_loader.h.
 * Native WaveArchiveFileReader is 12 bytes; game/archive.h's 16-byte
 * ArchiveReader has an additional field not present in the frozen native type. */
extern "C" {
const void *func_0016a29c(SoundLoaderState *loader, unsigned id);
const void *func_00183bfc(const ArchiveReader *reader, unsigned index);
const void *func_0016a888(const void *file, unsigned index,
                        const nw::snd::SoundArchive *archive, SoundLoaderState *loader);
}

const void *func_0016a888(const void *file, unsigned index,
                        const nw::snd::SoundArchive *archive, SoundLoaderState *loader)
{
    nw::snd::internal::WaveSoundFileReader resource(file);
    nw::snd::internal::WaveSoundNoteInfo info;
    /* ORIGINAL BUG: ignored lookup failure may leave wave identifiers uninitialized. */
    resource.ReadNoteInfo(&info, index, 0);
    const void *loaded_file = func_0016a29c(loader, info.waveArchiveId);
    if (loaded_file == 0)
        return 0;
    {
        nw::snd::SoundArchive::WaveArchiveInfo group;
        archive->ReadWaveArchiveInfo(info.waveArchiveId, &group);
        ArchiveReader reader;
        func_00183c84(&reader, static_cast<const ArchiveFile *>(loaded_file),
                      group.isLoadIndividual);
        return func_00183bfc(&reader, info.waveIndex);
    }
}
