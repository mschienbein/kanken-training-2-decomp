#include "game/archive.h"

struct SoundRuntime;


extern "C" unsigned func_00144088(const SoundRuntime *runtime, const SoundArchive *archive)
{
    SoundPlayerResult info;
    unsigned count = 0;
    if (func_00181334(archive, &info))
        count = info.stream_buffer_count;
    return count * 0xa000;
}
