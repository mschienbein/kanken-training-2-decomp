#include <preinclude.h>
#include <nw/ut/ut_Inlines.h>
#include "game/archive_queries.h"

static void ReadStreamInfo(struct ResourceBlock *reader);

extern "C" void func_001d47c8(struct ResourceBlock *reader, const struct ArchiveFile *file)
{
    if (file->signature != 0x4d545343)
        return;
    if (file->version - 0x02000000u > 0x00030100)
        return;
    reader->file = file;
    ReadStreamInfo(reader);
}

/* [INFERENCE] The canonical NW offset helper recovers the file-pointer
 * lifetime; original helper spelling and TU membership remain unrecovered. */
static void ReadStreamInfo(struct ResourceBlock *reader)
{
    const struct ArchiveFile *header = reader->file;
    reader->reference = reinterpret_cast<const RelativeReference *>(
        static_cast<const unsigned char *>(nw::ut::AddOffsetToPtr(header, func_001f1f20(header))) + 8);
}
