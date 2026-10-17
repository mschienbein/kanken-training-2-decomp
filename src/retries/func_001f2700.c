#include "game/sound_format.h"
#include "game/archive.h"









bool func_001f2700(const struct WaveSoundReader *reader, struct WaveSoundParameters *out, unsigned index)
{
    const struct WaveSoundData *data = func_00138fb0(reader->info, index);
    /* ORIGINAL BUG: a failed sound-data lookup is not checked before its subsequent use. */
    const struct SoundOptionalParameters *parameters = func_001f1904(data);
    unsigned char high;
    out->pitch = func_001f1af4(parameters);
    out->parameter0 = func_001f1ad0(&parameters->flags);
    out->parameter0_high = func_001f1aa8(parameters);
    func_001f1934(parameters, &out->send, out->sends, 2);
    out->envelope = *func_001f1a20(parameters);
    if (reader->file->version < 0x01000100)
        goto defaults;
    out->parameter2_low = func_001f1910(&parameters->flags);
    out->parameter2_middle = func_001f1a58(parameters);
    high = func_001f1a80(parameters);
finish:
    out->parameter2_high = high;
    return 1;
defaults:
    high = 0;
    out->parameter2_low = 64;
    out->parameter2_middle = high;
    goto finish;
}
