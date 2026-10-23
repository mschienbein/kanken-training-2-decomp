#ifndef GAME_ARCHIVE_LOADING_H
#define GAME_ARCHIVE_LOADING_H

#include "game/archive.h"
#include "game/archive_resource.h"

#ifdef __cplusplus
/* r3L00's retained bank-dependency loader supplies this two-word receiver
 * prefix and its file-lookup slot. The existing ArchiveAccess, BankReader,
 * resource-info and serialized table declarations remain canonical.
 * [INFERENCE] Loader/allocator identities and the bank-specific lookup role. */
struct ArchiveAllocator;
struct ArchiveLoader;
struct ArchiveLoaderVTable {
    unsigned char reserved00[0x10];
    const BankFile *(*find_file)(ArchiveLoader *, unsigned);
};
struct ArchiveLoader {
    const ArchiveLoaderVTable *vtable;
    ArchiveAccess *archive;
};
extern "C" {
unsigned func_0018d514(ArchiveHandle *handle, unsigned resource_id);
const ArchiveIdList *func_00140ec4(const ArchiveHandle *handle, unsigned resource_id);
const BankWaveArchiveTable *func_00185a6c(const BankReader *reader);
bool func_00183eb0(ArchiveAccess *archive, unsigned resource_id, ArchiveResourceInfo *out);
void *func_0018d728(ArchiveLoader *loader, unsigned file_id,
                    ArchiveAllocator *allocator, int block_size, int check_range);
bool func_00183d3c(ArchiveLoader *loader, unsigned resource_id, unsigned wave_archive_id,
                    ArchiveAllocator *allocator, int block_size);
bool func_0018d51c(ArchiveLoader *loader, unsigned resource_id, ArchiveAllocator *allocator,
                    unsigned flags, int block_size);
}
#endif

#endif
