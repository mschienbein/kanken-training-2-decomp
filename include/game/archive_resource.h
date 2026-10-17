#ifndef GAME_ARCHIVE_RESOURCE_H
#define GAME_ARCHIVE_RESOURCE_H
#include "game/archive.h"
struct ArchiveResourceReference {
    unsigned short type, reserved02;
    int offset;
};
struct ArchiveReferenceTable {
    unsigned count;
#ifndef __cplusplus
    struct ArchiveResourceReference entries[];
#endif
};
struct ArchiveInfoBlock {
    unsigned char reserved00[0x24];
    unsigned stream_table_offset;
    unsigned char reserved28[0x0c];
    unsigned file_table_offset;
};
/* [INFERENCE] This location reader is not equated with ArchiveIndex merely
 * because both metadata pointers occupy +0x3c: the file tables differ. */
struct ArchiveLocationReader {
    unsigned char reserved00[0x3c];
    const struct ArchiveInfoBlock *info;
};
struct ArchiveInternalFile { unsigned group, offset, size; };
struct ArchiveFileLocation { unsigned size, offset; const char *path; };
struct ArchiveResourceInfo { unsigned file_id, size; unsigned char external; };
#ifdef __cplusplus
extern "C" {
#endif
extern bool func_00140e88(const struct ArchiveIndex *index, unsigned resource_id,
                           struct ArchiveResourceInfo *out);
extern const struct ArchiveResourceReference *func_00144114(const struct ArchiveInfoBlock *info,
                                                            unsigned resource_id);
extern unsigned func_001441d4(const struct ArchiveResourceReference *reference);
extern const struct ArchiveInternalFile *func_001441f8(const struct ArchiveResourceReference *reference);
extern const char *func_001441a8(const struct ArchiveResourceReference *reference);
#ifdef __cplusplus
}
#endif
#endif
