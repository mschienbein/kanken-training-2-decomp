#ifndef GAME_ARCHIVE_H
#define GAME_ARCHIVE_H

struct SoundType1Details;
struct ArchiveAccess;
struct ArchiveProvider;
struct ArchiveProviderVTable;
struct ArchiveResourceInfo;
struct PackageAttribute;
struct PackageFile;
struct PackageView;
struct SoundType1Info;
struct SoundType3Details;
struct SoundType3Info;
struct SoundWaveFile;
struct SoundWaveInfo;
struct SoundWaveReader;
struct ArchiveHandleVTable;
struct BankFile;
struct SoundArchiveAddon;
struct SoundLookupProvider;














#include "game/archive_resource.h"

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
    unsigned short byte_order, header_size;
    unsigned version;                      /* 0x08 */
    unsigned file_size;
    unsigned short block_count, reserved12;
#ifdef __cplusplus
    /* The existing C++ clients use these two CWAR references only. This is
     * an observed prefix, not the complete extent of every resource file. */
    struct ArchiveBlockReference blocks[2];
#else
    /* Serialized files supply block_count records after the fixed header. */
    struct ArchiveBlockReference blocks[];
#endif
};

/* [INFERENCE] The stored tag is 16-bit, but the original comparisons consume
 * the full incoming core word. Do not truncate it at the shared endpoint. */
extern const struct ArchiveBlockReference *func_001650f8(
    const struct ArchiveFile *file, unsigned type);
extern unsigned func_001f1f20(const struct ArchiveFile *file);
extern unsigned func_001f1f34(const struct ArchiveFile *file);
extern unsigned func_001f1f4c(const struct ArchiveFile *file);
extern bool func_001f1ebc(const struct ArchiveFile *file);
extern bool func_001f1ed8(const struct ArchiveFile *file);

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


struct ArchiveHandle {
    const struct ArchiveHandleVTable *vtable;
    struct ArchiveIndex *index;
};

struct ArchiveIdList {
    unsigned count;
};

/* ARMCC C++ clients use these fixed prefixes only. C's zero-size flexible tails
 * describe the following serialized storage without changing either ABI layout. */


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

struct BankFile {
    unsigned signature, reserved04, version, reserved0c;
    unsigned short block_count, reserved12;
};
struct BankBlock { unsigned signature, size; struct SoundReference reference; };
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
    unsigned signature;
    struct RelativeReference contents;
    unsigned reserved0c[3];
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



struct SoundArchiveAddonVTable {
    unsigned char reserved00[0x20];
    bool (*read_file)(struct SoundArchiveAddon *, unsigned, struct SoundFileResult *);
    unsigned char reserved24[8];
    bool (*read_file_group)(struct SoundArchiveAddon *, unsigned, struct ArchiveResourceInfo *);
    void (*read_file_location)(struct SoundArchiveAddon *, unsigned, struct ArchiveFileLocation *, int);
    bool (*read_player)(struct SoundArchiveAddon *, struct SoundPlayerResult *);
    unsigned char reserved38[4];
    bool (*read_sequence)(struct SoundArchiveAddon *, unsigned, struct SoundType1Info *);
    unsigned char reserved40[8];
    bool (*read_wave_sound)(struct SoundArchiveAddon *, unsigned, struct SoundType3Info *);
};
struct SoundArchiveAddon {
    const struct SoundArchiveAddonVTable *vtable;
    bool enabled;
#ifdef __cplusplus
    bool read_file(unsigned id, SoundFileResult *out)
    {
        if (!enabled)
            return false;
        return vtable->read_file(this, id, out);
    }
    bool read_player(SoundPlayerResult *out)
    {
        if (!enabled)
            return false;
        return vtable->read_player(this, out);
    }
    bool read_sequence(unsigned id, SoundType1Info *out)
    {
        if (!enabled) return false;
        return vtable->read_sequence(this, id, out);
    }
    bool read_wave_sound(unsigned id, SoundType3Info *out)
    {
        if (!enabled) return false;
        return vtable->read_wave_sound(this, id, out);
    }
    void read_file_location(unsigned id, ArchiveFileLocation *out, int mode)
    {
        if (enabled) vtable->read_file_location(this, id, out, mode);
    }
#endif
};

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

extern bool func_0014a394(const struct SoundArchiveReader *reader, unsigned id,
                           struct SoundFileResult *out);
extern bool func_0014a3c4(const struct SoundArchiveReader *reader, struct SoundPlayerResult *out);
extern const struct ArchiveFileInfo *func_00144140(const struct ArchiveResourceTable *table,
                                                  unsigned resource_id);
extern unsigned func_00144188(const struct ArchiveFileInfo *info);
extern unsigned func_0014766c(const struct SoundContainerFile *file);
extern unsigned func_001476ac(const struct SoundContainerFile *file);
extern unsigned func_00140a08(struct ArchiveIndex *index, unsigned resource_id);
extern unsigned func_00140a10(const struct ArchiveResourceTable *table, unsigned resource_id);
extern const struct ArchiveIdList *func_00140ecc(const struct ArchiveIndex *index, unsigned resource_id);
extern const struct BankBlock *func_0014a440(const struct BankFile *file);
extern const struct BankWaveArchiveTable *func_00140fc0(const struct SoundReference *reference);
extern unsigned int func_0014a35c(const struct SoundArchive *archive);
extern unsigned int func_0014a364(const struct SoundArchiveReader *reader);
extern unsigned func_0014a380(const struct SoundArchiveReader *reader);
extern const unsigned *func_0014c298(const struct SoundArchiveInfo *info);
extern const unsigned *func_0014c2b0(const struct SoundArchiveInfo *info);
extern unsigned char func_00140ccc(const struct SoundRecord *record);
extern int func_00140d1c(const struct SoundRecord *record);
extern unsigned func_0014c23c(const struct SoundFileInfo *info);
extern const struct SoundFileInfo *func_0014c25c(const struct SoundArchiveInfo *info, unsigned id);
extern bool func_00181334(const struct SoundArchive *archive, struct SoundPlayerResult *out);
extern int func_0018279c(const unsigned *packed, unsigned *out, unsigned index);
extern int func_00131928(const struct SoundArchiveReader *reader, unsigned int sound_id);
extern const void *func_0016a938(const struct SoundArchivePlayerView *player, unsigned int sound_id);
extern const void *func_001f2ac8(const struct SoundArchiveReader *reader, unsigned int sound_id);

/* [INFERENCE] Natural callers consume the returned result-object pointer. */
extern struct WordPair *func_00194a34(struct WordPair *out, unsigned result);
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


struct SoundType3Info {
    unsigned first;
    unsigned second;
    unsigned char option_low;
    bool option_high_present;
};

struct SoundType1Info {
    unsigned option_zero;
    unsigned indices[4];
    unsigned property;
    unsigned char option_low;
    unsigned char option_high;
};



struct SoundWaveReader {
    const struct SoundWaveFile *file;
    const struct SoundWaveInfo *info;
    const void *data;
    unsigned char relocated;
};

struct SoundWaveFile {
    unsigned signature;
    unsigned short byte_order;
    unsigned short header_size;
    unsigned version;
    unsigned file_size;
    unsigned short block_count;
    unsigned short reserved12;
#ifndef __cplusplus
    struct ArchiveBlockReference blocks[];
#endif
};

struct ArchiveProviderVTable {
    unsigned char reserved00[0x2c];
    bool (*read_info)(struct ArchiveProvider *, unsigned, struct ArchiveResourceInfo *);
};

#ifdef __cplusplus
struct ArchiveProvider {
    const ArchiveProviderVTable *vtable;
    unsigned char enabled;

    bool read_resource_info(unsigned resource_id, ArchiveResourceInfo *out)
    {
        if (!enabled)
            return 0;
        return vtable->read_info(this, resource_id, out);
    }
};
#endif

#ifdef __cplusplus
struct ArchiveAccess {
    ArchiveHandle base;
    ArchiveProvider *provider;
    bool read_resource_info(unsigned resource_id, ArchiveResourceInfo *out);
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void func_00136cdc(struct WordPair *out, int year, int month, int day,
                          int hour, int minute, int second, int millisecond);
extern const struct SoundType3Details *func_00140d6c(const struct SoundRecord *record);
extern unsigned func_00140b9c(const struct SoundType3Details *details);
extern unsigned func_00140bc4(const struct SoundType3Details *details);
extern const struct SoundType1Details *func_00140da0(const struct SoundRecord *record);
extern unsigned func_00140c50(const struct SoundType1Details *details);
extern unsigned func_00140c70(const struct SoundType1Details *details);
extern unsigned func_00140c98(const struct SoundType1Details *details);
extern void func_00140bf0(const struct SoundType1Details *details, unsigned *output);
extern bool func_00139358(const struct SoundArchiveReader *reader, unsigned item_id,
                  struct SoundType1Info *output);
extern unsigned func_0012d4d0(void *destination, unsigned char mode,
                              unsigned long long handle,
                              const unsigned short *source, unsigned count);
extern const struct ArchiveBlockHeader *func_001f481c(const struct SoundWaveFile *file);
extern const struct ArchiveBlockHeader *func_001f47b4(const struct SoundWaveFile *file);
extern const struct ArchiveBlockHeader *func_001440b8(const struct ArchiveFile *file);
extern struct WordPair *func_001861ec(struct WordPair *out, int year, int month, int day,
                              int hour, int minute, int second, int millisecond);
extern bool func_001392b0(const struct SoundArchiveReader *reader, unsigned item_id,
                             struct SoundType3Info *output);
extern struct WordPair *func_00126554(struct WordPair *out,
                                         const struct PackageView *view,
                                         unsigned *transferred,
                                         void *destination, unsigned count,
                                         unsigned offset);
extern struct WordPair *func_00126988(struct WordPair *out,
                                         const struct PackageView *view,
                                         unsigned *transferred,
                                         void *destination, unsigned count,
                                         unsigned offset);
extern struct SoundWaveReader *func_001d3c78(struct SoundWaveReader *reader,
                                    const struct SoundWaveFile *file, unsigned char relocated);
extern struct WordPair *func_00194a34(struct WordPair *out, unsigned code);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
/* [INFERENCE] Native identities and roles; layouts follow matching consumers. */




extern "C" {
bool func_00140e14(const SoundArchiveReader *reader, unsigned file_id, ArchiveFileLocation *out, int mode);
bool func_00138edc(const SoundArchive *archive, unsigned file_id, ArchiveFileLocation *out);
bool func_00185884(const SoundArchive *archive, unsigned sound_id, SoundType3Info *out);
bool func_00185b44(const SoundArchive *archive, unsigned sound_id, SoundType1Info *out);
}
#endif

#endif
