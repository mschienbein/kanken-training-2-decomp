#ifndef GAME_SOUND_LOADER_H
#define GAME_SOUND_LOADER_H

struct SoundLoaderState;
struct DirectSoundLoaderState;
struct SoundLoaderCacheEntry;
struct CachedSoundLoaderState;
struct SoundLoaderVTable;

#include <stddef.h>
#include "game/archive.h"
#include "game/runtime.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */




struct SoundLoaderState {
    const SoundLoaderVTable *vtable;
    const SoundArchive *archive;
    unsigned char reserved08[0x200];
    unsigned short loaded_count;
    unsigned char state;
    unsigned char reserved20b;
};
struct DirectSoundLoaderState {
    const void *heap_vtable;
    unsigned used;
    unsigned limit;
    SoundLoaderState loader;
    void *file;
    void *allocation;
};
struct SoundLoaderCacheEntry {
    unsigned file_id;
    const void *file;
};
struct CachedSoundLoaderState {
    const void *heap_vtable;
    unsigned used;
    unsigned limit;
    SoundLoaderState loader;
    SoundLoaderCacheEntry cache[9];
    bool active;
    bool released;
    CachedSoundLoaderState();
    ~CachedSoundLoaderState();
};

extern "C" {

SoundLoaderState *func_001d4324(SoundLoaderState *loader);
void func_001d0324(DirectSoundLoaderState *owner);
void func_001d4174(SoundLoaderState *loader, const SoundArchive *archive);
void func_001d4124(CachedSoundLoaderState *owner, const SoundArchive *archive);
extern const SoundLoaderVTable data_00224810;
}
#endif

#endif
