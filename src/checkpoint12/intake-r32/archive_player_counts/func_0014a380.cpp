#include "game/archive.h"

/* [INFERENCE] Shared archive reader/helper and translation-unit identities. */
static const SoundArchiveInfo *archive_info(const SoundArchiveReader *reader)
{
    return reader->info;
}

template <typename Count>
static void read_count(unsigned *out, unsigned default_count,
                       unsigned minimum_version, const unsigned &version,
                       const Count *count)
{
    if (minimum_version <= version)
        *out = *count;
    else
        *out = default_count;
}

unsigned func_0014a380(const SoundArchiveReader *reader)
{
    return *func_0014c2b0(archive_info(reader));
}

bool func_0014a394(const SoundArchiveReader *reader, unsigned id, SoundFileResult *out)
{
    const SoundFileInfo *info = func_0014c25c(archive_info(reader), id);
    if (!info) return false;
    out->count = info->count;
    out->size = func_0014c23c(info);
    return true;
}

bool func_0014a3c4(const SoundArchiveReader *reader, SoundPlayerResult *out)
{
    const SoundPlayerInfo *info = func_0014c2a4(archive_info(reader));
    if (!info) return false;
    read_count(&out->sound_count, 0, 0, 0, &info->sound_count);
    read_count(&out->sequence_count, 0, 0, 0, &info->sequence_count);
    read_count(&out->stream_count, 0, 0, 0, &info->stream_count);
    read_count(&out->bank_count, 0, 0, 0, &info->bank_count);
    read_count(&out->stream_buffer_count, 0, 0, 0, &info->stream_buffer_count);
    read_count(&out->wave_sound_count, 0, 0, 0, &info->wave_sound_count);
    read_count(&out->wave_archive_count, 0, 0, 0, &info->wave_archive_count);
    read_count(&out->stream_buffer_times, 1, 0x02030200, reader->version,
               &info->stream_buffer_times);
    return true;
}
