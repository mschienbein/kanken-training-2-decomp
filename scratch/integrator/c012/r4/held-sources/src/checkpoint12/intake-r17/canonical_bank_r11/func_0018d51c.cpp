#include "game/archive_loading.h"


static const BankWaveArchiveEntry *bank_wave_archive_entry(const BankWaveArchiveTable *table, unsigned index)
{
    if (table->count <= index)
        return 0;
    return &reinterpret_cast<const BankWaveArchiveEntry *>(table + 1)[index];
}

static bool load_internal_archive(ArchiveLoader *loader, unsigned resource_id, ArchiveAllocator *allocator, unsigned flags, int block_size)
{
    if (flags & 8) {
        unsigned file_id = func_0018d514(&loader->archive->base, resource_id);
        if (func_0018d728(loader, file_id, allocator, block_size, 1) == 0)
            return 0;
    }
    return 1;
}

static bool load_bank_dependency(ArchiveLoader *loader, const BankWaveArchiveEntry *entry,
    ArchiveAllocator *allocator, unsigned flags, int block_size)
{
    unsigned archive_id = entry->resource_id;
    unsigned wave_archive_id = entry->wave_archive_id;
    ArchiveResourceInfo info;
    info.file_id = ~0u;
    info.external = 0;
    if (!func_00183eb0(loader->archive, archive_id, &info))
        return 0;
    if (info.external) {
        if (!func_00183d3c(loader, archive_id, wave_archive_id, allocator, block_size))
            return 0;
    } else if (!load_internal_archive(loader, archive_id, allocator, flags, block_size)) {
        return 0;
    }
    return 1;
}

bool func_0018d51c(ArchiveLoader *loader, unsigned resource_id, ArchiveAllocator *allocator, unsigned flags, int block_size)
{
    unsigned file_id = func_0018d514(&loader->archive->base, resource_id);
    if ((flags & 4) && func_0018d728(loader, file_id, allocator, block_size, 0) == 0)
        return false;
    if (flags & 8) {
        const BankFile *file = loader->vtable->find_file(loader, file_id);
        if (file != 0) {
            BankReader reader(file);
            const BankWaveArchiveTable *table = func_00185a6c(&reader);
            if (table == 0)
                return false;
            for (unsigned i = 0; i < table->count; ++i) {
                const BankWaveArchiveEntry *source_entry = bank_wave_archive_entry(table, i);
                if (source_entry == 0)
                    return false;
                BankWaveArchiveEntry entry = *source_entry;
                if (!load_bank_dependency(loader, &entry, allocator, flags, block_size))
                    return false;
            }
        } else {
            const ArchiveIdList *list = func_00140ec4(&loader->archive->base, resource_id);
            /* ORIGINAL BUG: the fallback list is dereferenced without checking for a failed lookup. */
            for (unsigned i = 0; i < list->count; ++i) {
                unsigned id = reinterpret_cast<const unsigned *>(list)[i + 1];
                if (!load_internal_archive(loader, id, allocator, flags, block_size))
                    return false;
            }
        }
    }
    return true;
}
