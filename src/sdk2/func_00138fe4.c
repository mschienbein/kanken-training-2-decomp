#include "game/sound_format.h"
const struct WaveNoteInfo *func_00138fe4(const struct WaveSoundData *data, unsigned index)
{
    const struct SoundReferenceTable *table = (const struct SoundReferenceTable *)
        ((const unsigned char *)data + data->notes.offset);
    unsigned note_type = 0x4902;
    if (index >= table->count)
        return 0;
    if (table->entries[index].type != note_type)
        return 0;
    return (const struct WaveNoteInfo *)
        ((const unsigned char *)table + table->entries[index].offset);
}
