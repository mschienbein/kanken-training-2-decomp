#include "game/archive.h"
#include "game/archive_ipc.h"
#include <new>
static ServiceResult *operation_result(ServiceResult *result, unsigned code)
{
    return new (result) ServiceResult(Result(code));
}
/* [INFERENCE] Read cursor and helper identities are unrecovered. */
struct ArchiveReadCursor { unsigned *count; unsigned index; };
static unsigned read_archive(const struct FileArchiveHandle *archive,
    struct ArchiveReadCursor cursor, struct ArchiveInfo *entries,
    unsigned capacity)
{
    return func_0011f6a8(cursor.count, entries, capacity, 1,
        archive->value, cursor.index + 1);
}
extern "C" ServiceResult *func_00117ad8(ServiceResult *result,
    const FileArchiveHandle *archive, unsigned *count,
    ArchiveInfo *entries, unsigned capacity, unsigned index)
{
    unsigned code;
    struct ArchiveReadCursor cursor;
    if (archive->value == 0)
        return operation_result(result, 0xe0a13882u);
    cursor.count = count;
    cursor.index = index;
    code = read_archive(archive, cursor, entries, capacity);
    return operation_result(result, code);
}
