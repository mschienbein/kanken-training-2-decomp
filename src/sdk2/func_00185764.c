#include "game/archive.h"

struct SoundBinaryReference {
    unsigned short type;
    unsigned short reserved;
    int offset;
};

struct WaveSoundInfoBlock {
    struct SoundBinaryReference wave_ids;
    struct SoundBinaryReference wave_sound_data;
};

struct WaveSoundReader {
    const struct ArchiveFile *file;
    const struct WaveSoundInfoBlock *info;
};

union SoundParameterValue {
    unsigned word;
    float real;
};

struct WaveNoteInfo {
    unsigned wave_identifier;
    unsigned flags;
    union SoundParameterValue values[];
};

struct WaveIdentifier {
    unsigned archive_id;
    unsigned wave_index;
};

struct WaveIdentifierTable {
    unsigned count;
    struct WaveIdentifier entries[];
};

struct SoundEnvelope {
    unsigned char attack;
    unsigned char decay;
    unsigned char sustain;
    unsigned char hold;
    unsigned char release;
};

struct WaveNoteParameters {
    struct WaveIdentifier wave;
    struct SoundEnvelope envelope;
    unsigned char parameter0;
    unsigned char parameter2_low;
    unsigned char parameter2_high;
    unsigned char parameter1;
    unsigned char reserved11[3];
    float pitch;
};

struct WaveSoundData;
extern const struct WaveSoundData *func_00138fb0(const struct WaveSoundInfoBlock *info, unsigned index);
extern const struct WaveNoteInfo *func_00138fe4(const struct WaveSoundData *data, unsigned index);
extern const struct WaveIdentifierTable *func_00138fa4(const struct WaveSoundInfoBlock *info);
extern float func_001390d8(const struct WaveNoteInfo *note);
extern const struct SoundEnvelope *func_00139020(const struct WaveNoteInfo *note);
extern unsigned char func_0013905c(const struct WaveNoteInfo *note);
extern unsigned char func_001390b0(const struct WaveNoteInfo *note);
extern unsigned char func_00139084(const struct WaveNoteInfo *note);
extern unsigned char func_0013914c(const struct WaveNoteInfo *note);

int func_00185764(const struct WaveSoundReader *reader, struct WaveNoteParameters *out,
                  unsigned data_index, unsigned note_index)
{
    const struct WaveSoundData *data = func_00138fb0(reader->info, data_index);
    /* ORIGINAL BUG: failed record lookups are not checked before their subsequent use. */
    const struct WaveNoteInfo *note = func_00138fe4(data, note_index);
    const struct WaveIdentifierTable *table = func_00138fa4(reader->info);
    const struct WaveIdentifier *wave;
    if (table->count <= note->wave_identifier)
        wave = 0;
    else
        wave = &table->entries[note->wave_identifier];
    if (!wave)
        return 0;
    out->wave.archive_id = wave->archive_id;
    out->wave.wave_index = wave->wave_index;
    out->pitch = func_001390d8(note);
    out->envelope = *func_00139020(note);
    out->parameter0 = func_0013905c(note);
    out->parameter2_low = func_001390b0(note);
    out->parameter2_high = func_00139084(note);
    out->parameter1 = func_0013914c(note);
    return 1;
}
