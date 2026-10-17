#ifndef GAME_ARCHIVE_H
#define GAME_ARCHIVE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Partial CWAR resource-file views. Only accessed fields are named. */
struct ArchiveBlockReference {
    unsigned short type;                   /* 0x00 */
    unsigned char reserved02[2];
    unsigned offset;                       /* 0x04 */
    unsigned size;                         /* 0x08 */
};

struct ArchiveFile {
    unsigned signature;                    /* 0x00 */
    unsigned char reserved04[4];
    unsigned version;                      /* 0x08 */
    unsigned char reserved0c[8];
    struct ArchiveBlockReference blocks[2]; /* 0x14; references at 0x14 and 0x20 */
};

struct SoundContainerFile {
    unsigned char reserved00[0x14];
    struct ArchiveBlockReference blocks[3];
};

struct ArchiveBlockHeader {
    unsigned signature;                    /* 0x00 */
    unsigned char reserved04[4];
};

struct ArchiveInfoTable {
    unsigned count;
};
struct ArchiveReader {
    const struct ArchiveFile *file;         /* 0x00 */
    const struct ArchiveInfoTable *info;    /* 0x04 */
    const void **cache;                     /* 0x08 */
    unsigned char initialized;             /* 0x0c */
};

/* A typed, self-relative resource reference; unsupported types resolve to null. */
struct RelativeReference {
    unsigned short type;
    unsigned short reserved;
    int offset;
};

struct ResourceBlock {
    const struct ArchiveFile *file;
    const struct RelativeReference *reference;
};

/* Two consecutive words, also used by the pair setter at 0x00136448. */
struct WordPair {
    unsigned int first;
    unsigned int second;
};

/* Indexed resource archive, separate from the CWAR reader above. */
struct ArchiveResourceTable {
    unsigned reserved00[7];
    unsigned file_table_offset;
};

struct ArchiveIndex {
    unsigned reserved00[15];
    const struct ArchiveResourceTable *table;
};

struct ArchiveHandleVTable;
struct ArchiveHandle {
    const struct ArchiveHandleVTable *vtable;
    struct ArchiveIndex *index;
};

struct ArchiveIdList {
    unsigned count;
};

/* ARMCC C++ clients use these fixed prefixes only. C's zero-size flexible tails
 * describe the following serialized storage without changing either ABI layout. */

struct ArchiveSoundInfo {
    unsigned reserved00[3];
#ifndef __cplusplus
    unsigned parameters[];
#endif
};

struct ArchiveStreamInfo {
    unsigned reserved00[5];
#ifndef __cplusplus
    unsigned parameters[];
#endif
};

struct SoundRecord {
    unsigned item_id;
    unsigned field04;
    unsigned char property;
    unsigned char reserved09[3];
    unsigned short type;
    unsigned short reserved0e;
    int detail_offset;
    unsigned option_flags;
};

struct ArchiveFileReference {
    unsigned type;
    unsigned offset;
};

struct ArchiveFileDirectory {
    unsigned count;
#ifndef __cplusplus
    struct ArchiveFileReference entries[];
#endif
};

struct ArchiveFileInfo {
    unsigned file_id;
    unsigned char external;
    unsigned char reserved05[3];
#ifndef __cplusplus
    unsigned parameters[];
#endif
};

struct BankFile;
struct SoundReference {
    unsigned short type;
    unsigned short reserved02;
    int offset;
};

struct SoundReferenceTable {
    unsigned count;
#ifndef __cplusplus
    struct SoundReference entries[];
#endif
};

struct BankReader {
    const struct BankFile *file;
    const struct SoundReference *info;
    unsigned char valid;
};

struct BankWaveArchiveEntry {
    unsigned resource_id;
    unsigned wave_archive_id;
};

struct BankWaveArchiveTable {
    unsigned count;
};

struct BankInstrumentInfo {
    unsigned reserved00[6];
#ifndef __cplusplus
    unsigned parameters[];
#endif
};

/* Sound archive runtime object and its serialized metadata. */
struct SoundArchiveInfo {
    unsigned reserved00;
    int sound_table_offset;
    unsigned reserved08;
    int range_table_offset;
    unsigned reserved10;
    int third_table_offset;
    unsigned char reserved18[0x14];
    unsigned file_table_offset;
    unsigned reserved30;
    unsigned count_table_offset;
    unsigned reserved38;
    unsigned player_info_offset;
};

struct SoundArchiveReader {
    const void *vtable;
    unsigned reserved04;
    unsigned version;
    unsigned char reserved0c[0x30];
    const struct SoundArchiveInfo *info;
};

struct SoundArchiveAddon;
struct SoundArchive {
    const void *vtable;
    const struct SoundArchiveReader *reader;
    struct SoundArchiveAddon *addon;
};

struct SoundFileInfo {
    unsigned count;
#ifndef __cplusplus
    unsigned parameters[];
#endif
};

struct SoundInfoReference {
    unsigned type;
    unsigned offset;
};

struct SoundInfoTable {
    unsigned count;
#ifndef __cplusplus
    struct SoundInfoReference entries[];
#endif
};

struct SoundFileResult {
    unsigned count;
    unsigned size;
};

struct SoundPlayerInfo {
    unsigned short sound_count;
    unsigned short sequence_count;
    unsigned short stream_count;
    unsigned short bank_count;
    unsigned short stream_buffer_count;
    unsigned short wave_sound_count;
    unsigned short wave_archive_count;
    unsigned char stream_buffer_times;
};

struct SoundPlayerResult {
    unsigned sound_count;
    unsigned sequence_count;
    unsigned stream_count;
    unsigned bank_count;
    unsigned stream_buffer_count;
    unsigned wave_sound_count;
    unsigned wave_archive_count;
    unsigned stream_buffer_times;
};

struct SoundLookupProvider;
struct SoundLookupVTable {
    unsigned char reserved00[0x0c];
    const void *(*lookup)(struct SoundLookupProvider *provider, unsigned int sound_id);
};

struct SoundLookupProvider {
    const struct SoundLookupVTable *vtable;
    unsigned char enabled;
};

struct SoundArchivePlayerView {
    const void *reserved00;
    const struct SoundArchiveReader *archive;
    struct SoundLookupProvider *provider;
};

extern unsigned func_00140a08(struct ArchiveIndex *index, unsigned resource_id);
extern unsigned func_00140a10(const struct ArchiveResourceTable *table, unsigned resource_id);
extern const struct ArchiveIdList *func_00140ecc(const struct ArchiveIndex *index, unsigned resource_id);
extern const struct BankWaveArchiveTable *func_00140fc0(const struct SoundReference *reference);
extern unsigned int func_0014a35c(const struct SoundArchive *archive);
extern unsigned int func_0014a364(const struct SoundArchiveReader *reader);
extern unsigned func_0014a380(const struct SoundArchiveReader *reader);
extern const unsigned *func_0014c298(const struct SoundArchiveInfo *info);
extern const unsigned *func_0014c2b0(const struct SoundArchiveInfo *info);
extern unsigned func_0014c23c(const struct SoundFileInfo *info);
extern const struct SoundFileInfo *func_0014c25c(const struct SoundArchiveInfo *info, unsigned id);
extern bool func_00181334(const struct SoundArchive *archive, struct SoundPlayerResult *out);
extern int func_0018279c(const unsigned *packed, unsigned *out, unsigned index);
extern int func_00131928(const struct SoundArchiveReader *reader, unsigned int sound_id);
extern const void *func_0016a938(const struct SoundArchivePlayerView *player, unsigned int sound_id);
extern const void *func_001f2ac8(const struct SoundArchiveReader *reader, unsigned int sound_id);

extern void func_00136448(struct WordPair *pair, unsigned int first, unsigned int second);
extern const void *func_0017ded4(const struct RelativeReference *reference);

extern const struct ArchiveBlockHeader *func_0014a2f0(const struct ArchiveFile *file);
extern unsigned func_00183c54(const struct ArchiveFile *file);
extern struct ArchiveReader *func_00183c84(struct ArchiveReader *reader,
                                          const struct ArchiveFile *file, int use_table);
extern void func_00140970(struct ArchiveReader *reader);

#ifdef __cplusplus
}
#endif

#endif
