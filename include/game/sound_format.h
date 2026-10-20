#ifndef GAME_SOUND_FORMAT_H
#define GAME_SOUND_FORMAT_H

struct ArchiveBlockHeader;
struct ArchiveBlockReference;
struct ArchiveFile;
struct SoundArchiveInfo;
struct SoundRecord;
struct SoundReference;
struct WaveSoundParameters;

#include <stdbool.h>

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
struct SequenceLabelEntry { unsigned reserved00, offset; };
struct SequenceLabelTable {
    int count;
#ifndef __cplusplus
    struct SequenceLabelEntry entries[];
#endif
};
struct SequenceLabelRecord {
    unsigned reserved00, value, reserved08;
#ifndef __cplusplus
    char name[];
#endif
};
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
#ifdef __cplusplus
    SoundEnvelope(unsigned char a = 0, unsigned char d = 0, unsigned char s = 0,
                  unsigned char h = 0, unsigned char r = 0)
        : attack(a), decay(d), sustain(s), hold(h), release(r) {}
#endif
};

struct WaveSoundReader {
    const struct ArchiveFile *file;
    const struct WaveSoundInfoBlock *info;
};
struct WaveNoteParameters {
    struct WaveIdentifier wave;
    struct SoundEnvelope envelope;
    unsigned char parameter0, parameter2_low, parameter2_high, parameter1;
    unsigned char reserved11[3];
    float pitch;
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
extern const struct SoundReferenceTable *func_0014417c(const struct SoundArchiveInfo *info);
extern const struct WaveSoundData *func_00138fb0(const struct WaveSoundInfoBlock *info, unsigned index);
extern const struct WaveNoteInfo *func_00138fe4(const struct WaveSoundData *data, unsigned index);
extern const struct WaveIdentifierTable *func_00138fa4(const struct WaveSoundInfoBlock *info);
extern float func_001390d8(const struct WaveNoteInfo *note);
extern const struct SoundEnvelope *func_00139020(const struct WaveNoteInfo *note);
extern unsigned char func_0013905c(const struct WaveNoteInfo *note);
extern unsigned char func_001390b0(const struct WaveNoteInfo *note);
extern unsigned char func_00139084(const struct WaveNoteInfo *note);
extern unsigned char func_0013914c(const struct WaveNoteInfo *note);
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


struct WaveSoundParameters {
    float pitch;
    struct SoundEnvelope envelope;
    unsigned char parameter0;
    signed char parameter0_high;
    unsigned char send;
    unsigned char sends[2];
    unsigned char parameter2_low;
    unsigned char parameter2_middle;
    unsigned char parameter2_high;
};

#ifdef __cplusplus
extern "C" {
#endif
extern const struct SoundOptionalParameters *func_001f1904(const struct WaveSoundData *data);
extern float func_001f1af4(const struct SoundOptionalParameters *note);
extern unsigned char func_001f1ad0(const unsigned *parameters);
extern signed char func_001f1aa8(const struct SoundOptionalParameters *note);
extern void func_001f1934(const struct SoundOptionalParameters *note, unsigned char *send,
                         unsigned char *sends, unsigned count);
extern const struct SoundEnvelope *func_001f1a20(const struct SoundOptionalParameters *note);
extern unsigned char func_001f1910(const unsigned *parameters);
extern unsigned char func_001f1a58(const struct SoundOptionalParameters *note);
extern unsigned char func_001f1a80(const struct SoundOptionalParameters *note);
extern bool func_001f2700(const struct WaveSoundReader *reader, struct WaveSoundParameters *out, unsigned index);
#ifdef __cplusplus
}
#endif

#endif
