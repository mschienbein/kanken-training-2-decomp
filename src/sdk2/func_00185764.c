#include "game/sound_format.h"
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
