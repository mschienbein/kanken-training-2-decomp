#include "game/sound_format.h"

struct ArchiveAllocator {
    virtual void reserved00() = 0;
    virtual void reserved04() = 0;
    virtual void *allocate(unsigned size) = 0;
};
struct ArchiveLoader {
    virtual void reserved00() = 0;
    virtual void reserved04() = 0;
    virtual void register_file(unsigned file_id, void *buffer) = 0;
    virtual void reserved0c() = 0;
    virtual void *find_file(unsigned file_id) = 0;
    SoundArchive *archive;
};
struct SequenceSoundInfo {
    unsigned offset;
    unsigned bank_ids[4];
    unsigned channel_flags;
    unsigned char release_priority, allocation_track;
    unsigned char reserved1a[2];
    SequenceSoundInfo() : offset(0), channel_flags(0), release_priority(0), allocation_track(0)
    {
        for (unsigned i = 0; i < 4; ++i)
            bank_ids[i] = ~0u;
    }
};
/* INTEGRATOR: move to include/game/archive.h */
extern "C" {
bool func_00131824(const SoundArchive *archive, unsigned group_id, SoundRangeInfo *out);
/* INTEGRATOR: differs from src/wave6/func_00131920.c: the caller passes the same SoundArchive consumed by the metadata adapters. */
int func_00131920(const SoundArchive *archive, unsigned int sound_id);
/* INTEGRATOR: differs from src/wave4/func_0018d514.c: the loader supplies its SoundArchive, not a separately typed two-word handle view. */
unsigned func_0018d514(SoundArchive *archive, unsigned resource_id);
bool func_00185b44(const SoundArchive *archive, unsigned sound_id, SequenceSoundInfo *out);
bool func_0018d51c(ArchiveLoader *loader, unsigned bank_id, ArchiveAllocator *allocator,
                    unsigned load_flags, unsigned read_flags);
void *func_0018d728(ArchiveLoader *loader, unsigned file_id, ArchiveAllocator *allocator,
                    unsigned flags, bool require_device_memory);
bool func_00131030(ArchiveLoader *loader, unsigned sound_id, ArchiveAllocator *allocator,
                    unsigned load_flags, unsigned read_flags, unsigned group_id);
bool func_001252b0(ArchiveLoader *loader, unsigned group_id, ArchiveAllocator *allocator,
                    unsigned load_flags, unsigned read_flags);
}

static SoundRangeInfo empty_sound_range()
{
    SoundRangeInfo info;
    info.first_id = ~0u;
    info.last_id = ~0u;
    info.data = 0;
    return info;
}

static bool PrepareSequence(ArchiveLoader *loader, unsigned sound_id, ArchiveAllocator *allocator,
                            unsigned load_flags, unsigned read_flags)
{
    if (load_flags & 1) {
        unsigned file_id = func_0018d514(loader->archive, sound_id);
        if (!func_0018d728(loader, file_id, allocator, read_flags, false))
            return false;
    }
    if (load_flags & 12) {
        SequenceSoundInfo info;
        if (!func_00185b44(loader->archive, sound_id, &info))
            return false;
        for (unsigned i = 0; i < 4; ++i)
            if (info.bank_ids[i] != ~0u &&
                !func_0018d51c(loader, info.bank_ids[i], allocator, load_flags, read_flags))
                return false;
    }
    return true;
}

bool func_001252b0(ArchiveLoader *loader, unsigned group_id, ArchiveAllocator *allocator,
                    unsigned load_flags, unsigned read_flags)
{
    SoundRangeInfo info = empty_sound_range();
    if (!func_00131824(loader->archive, group_id, &info))
        return false;
    if (info.first_id == ~0u)
        return true;
    int kind = func_00131920(loader->archive, info.first_id);
    if (kind == 1) {
        for (unsigned id = info.first_id; id <= info.last_id; ++id)
            if (!PrepareSequence(loader, id, allocator, load_flags, read_flags))
                return false;
    } else if (kind == 3) {
        for (unsigned id = info.first_id; id <= info.last_id; ++id)
            if (!func_00131030(loader, id, allocator, load_flags, read_flags, group_id))
                return false;
    }
    return true;
}
