#ifndef GAME_SOUND_FORMAT_H
#define GAME_SOUND_FORMAT_H

#include "game/archive.h"

struct SoundBankArchiveInfo {
    unsigned file_id;
};

struct ArchiveStreamEntry {
    unsigned file_id;
};

struct SequenceFile {
    unsigned signature;
    unsigned char reserved04[4];
    unsigned version;
    unsigned char reserved0c[4];
    unsigned short block_count;
    unsigned char reserved12[2];
#ifndef __cplusplus
    struct ArchiveBlockReference blocks[];
#endif
};

struct SequenceLabelTable;
struct SequenceReader {
    const struct SequenceFile *file;
    const unsigned char *data;
    const struct SequenceLabelTable *labels;
};

union SoundParameterValue {
    unsigned word;
    float real;
};

struct WaveSoundInfoBlock {
    struct SoundReference wave_ids;
    struct SoundReference wave_sound_data;
};

struct WaveIdentifier {
    unsigned archive_id;
    unsigned wave_index;
};

struct WaveIdentifierTable {
    unsigned count;
#ifndef __cplusplus
    struct WaveIdentifier entries[];
#endif
};

struct WaveSoundData {
    struct SoundReference options;
    unsigned char reserved08[8];
    struct SoundReference notes;
};

struct WaveNoteInfo {
    unsigned wave_identifier;
    unsigned flags;
#ifndef __cplusplus
    union SoundParameterValue values[];
#endif
};

struct SoundOptionalParameters {
    unsigned flags;
#ifndef __cplusplus
    union SoundParameterValue values[];
#endif
};

struct SoundEnvelope {
    unsigned char attack;
    unsigned char decay;
    unsigned char sustain;
    unsigned char hold;
    unsigned char release;
};

struct SoundBasicInfo {
    unsigned item_id;
    unsigned field04;
    unsigned option_two_high;
    unsigned option_two_low;
    unsigned property;
    unsigned char option_one_low;
    unsigned char option_one_high;
    unsigned char flag;
};

struct SoundRangeRecord {
    unsigned first_id;
    unsigned last_id;
    unsigned reserved08;
    int name_offset;
    unsigned short type;
    unsigned short reserved12;
    int detail_offset;
};

struct SoundRangeInfo {
    unsigned first_id;
    unsigned last_id;
    const void *data;
};

struct SoundType3Details {
    unsigned first;
    unsigned second;
    unsigned option_flags;
};

struct SoundType1Details {
    unsigned reserved00;
    int indices_offset;
    unsigned property;
    unsigned option_flags;
};

struct SoundIndexList {
    unsigned count;
#ifndef __cplusplus
    unsigned indices[];
#endif
};

struct SoundBankInfo {
    struct SoundReference wave_table;
    struct SoundReference instrument_table;
};

struct SoundRegion {
    unsigned reference;
    unsigned option_flags;
#ifndef __cplusplus
    unsigned parameters[];
#endif
};

struct SoundRegionFixed;
#ifdef __cplusplus
extern "C" {
#endif
extern const struct SoundEnvelope data_0024380a;
extern const struct SoundEnvelope data_002437f8;
extern const struct SoundBankArchiveInfo *func_001409cc(
    const struct SoundArchiveInfo *info, unsigned bank_id);
extern const struct ArchiveBlockHeader *func_00153de8(const struct SequenceFile *file);
extern const struct ArchiveBlockHeader *func_00153e54(const struct SequenceFile *file);
extern int func_001f240c(const struct SequenceLabelTable *labels,
                         const char *name, unsigned *offset);
extern const struct SoundRecord *func_0018341c(const struct SoundArchiveInfo *info,
                                              unsigned item_id);
extern const struct SoundRangeRecord *func_00182760(const struct SoundArchiveInfo *info,
                                                   unsigned item_id);
extern unsigned func_001833ec(const struct SoundRecord *record);
extern unsigned char func_00140ccc(const struct SoundRecord *record);
extern unsigned func_00140cf0(const struct SoundRecord *record);
extern int func_00140d1c(const struct SoundRecord *record);
extern unsigned func_00140d40(const struct SoundRecord *record);
extern unsigned func_00140d78(const struct SoundRecord *record);
#ifdef __cplusplus
}
#endif

#endif
