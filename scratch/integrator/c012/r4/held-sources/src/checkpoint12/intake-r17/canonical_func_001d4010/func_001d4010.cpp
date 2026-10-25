#include "game/archive_stream.h"
#include <new>

ArchiveTransfer *func_001d4010(ArchiveStreamObject *object,
    ArchiveTransfer *storage, unsigned int size, int first_option, int second_option)
{
    if (object->open && size >= sizeof(ArchiveTransfer)) {
        ArchiveTransfer *transfer = new (storage) ArchiveTransfer(&object->context,
            first_option, second_option, func_001d4078, object);
        /* ORIGINAL BUG: a null storage pointer still reaches the validity read. */
        if (transfer->valid)
            return transfer;
    }
    return 0;
}
