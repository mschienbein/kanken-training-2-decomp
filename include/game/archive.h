#ifndef GAME_ARCHIVE_H
#define GAME_ARCHIVE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Partial CWAR resource-file views. Only accessed fields are named. */
struct ArchiveBlockReference {
    unsigned short type;                   /* 0x00 */
    unsigned char reserved02[2];
    unsigned offset;                       /* 0x04 */
    unsigned char reserved08[4];
};

struct ArchiveFile {
    unsigned signature;                    /* 0x00 */
    unsigned char reserved04[4];
    unsigned version;                      /* 0x08 */
    unsigned char reserved0c[8];
    struct ArchiveBlockReference blocks[2]; /* 0x14; references at 0x14 and 0x20 */
};

struct ArchiveBlockHeader {
    unsigned signature;                    /* 0x00 */
    unsigned char reserved04[4];
};

struct ArchiveInfoTable;
struct ArchiveReader {
    const struct ArchiveFile *file;         /* 0x00 */
    const struct ArchiveInfoTable *info;    /* 0x04 */
    const void **cache;                     /* 0x08 */
    unsigned char initialized;             /* 0x0c */
};

extern const struct ArchiveBlockHeader *func_0014a2f0(const struct ArchiveFile *file);
extern unsigned func_00183c54(const struct ArchiveFile *file);
extern struct ArchiveReader *func_00183c84(struct ArchiveReader *reader,
                                          const struct ArchiveFile *file, int use_table);
extern void func_00140970(struct ArchiveReader *reader);

#ifdef __cplusplus
}
#endif

#endif
