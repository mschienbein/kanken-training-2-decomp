#include "game/sound_loader.h"
#include <stddef.h>
#include "game/archive.h"

struct SoundLoaderVTable;

struct PrimarySoundLoaderVTable {
    const void *entries[8];
};

/* INTEGRATOR: move to include/game/sound_loader.h */
extern "C" {
extern const PrimarySoundLoaderVTable data_00224854;
/* INTEGRATOR: differs from src/wave8/func_001d4174.c: this caller uses the
 * SoundArchiveLoader layout (+4 archive pointer), not ArchiveHandle ownership.
 * Reconcile with the native nw::snd::internal::SoundArchiveLoader declaration. */


CachedSoundLoaderState *func_0016a104(CachedSoundLoaderState *owner);
}

static void initialize_tables(CachedSoundLoaderState *owner,
                              const PrimarySoundLoaderVTable *tables)
{
    owner->heap_vtable = tables;
    owner->loader.vtable = reinterpret_cast<const SoundLoaderVTable *>(tables + 1);
}

static void release_loader(CachedSoundLoaderState *owner)
{
    if (!owner->released) {
        owner->active = false;
        owner->released = true;
        /* [INFERENCE] callee identity: inlined by armlink. */
        func_001d4174(&owner->loader, 0);
    }
}

CachedSoundLoaderState *func_0016a104(CachedSoundLoaderState *owner)
{
    initialize_tables(owner, &data_00224854);
    release_loader(owner);
    /* The ABI destructor returns the secondary base; recover its owner. */
    SoundLoaderState *base = func_001d4324(&owner->loader);
    return reinterpret_cast<CachedSoundLoaderState *>(
        reinterpret_cast<char *>(base) - offsetof(CachedSoundLoaderState, loader));
}
