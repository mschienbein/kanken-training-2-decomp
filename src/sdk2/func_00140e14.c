#include "game/archive_resource.h"
_Bool func_00140e14(struct ArchiveLocationReader *index, unsigned resource_id,
                  struct ArchiveFileLocation *location)
{
    const struct ArchiveResourceReference *reference = func_00144114(index->info, resource_id);
    if (reference) {
        switch (func_001441d4(reference)) {
        case 0: {
            const struct ArchiveInternalFile *file = func_001441f8(reference);
            location->size = file->size;
            location->offset = file->offset == ~0u ? ~0u : file->offset + 8;
            break;
        }
        case 1:
            location->path = func_001441a8(reference);
            break;
        case 2:
            return 0;
        }
        return 1;
    }
    return 0;
}
