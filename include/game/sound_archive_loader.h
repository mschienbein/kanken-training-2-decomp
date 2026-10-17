#ifndef GAME_SOUND_ARCHIVE_LOADER_H
#define GAME_SOUND_ARCHIVE_LOADER_H
#include "game/archive.h"
#include "game/memory_file.h"
#include "game/resource_stream.h"
struct SoundArchiveLoaderStateW9J {
    unsigned char reserved00[0x1c];
    ResourceFile file;
    union { SoundContainerFile info; MemoryFileReaderW9J memory; } container;
    unsigned reserved7c;
    Result result;
};
struct SoundArchiveLoaderW9J {
    unsigned char reserved00[0x100];
    SoundArchiveLoaderStateW9J state;
};
extern "C" int func_00143b28(SoundArchiveLoaderW9J *loader, void *buffer, unsigned capacity);
#endif
