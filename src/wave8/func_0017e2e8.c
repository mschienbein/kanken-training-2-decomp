#include "game/sound_format.h"

struct SequenceReader *func_0017e2e8(struct SequenceReader *reader,
                                   const struct SequenceFile *file)
{
    reader->file = 0;
    reader->data = 0;
    reader->labels = 0;
    if (file->signature != 0x51455343)
        goto invalid;
    if (file->version - 0x01000000u > 0x10000u)
        goto invalid;
    reader->file = file;
    /* ORIGINAL BUG: valid file headers are assumed to contain both mandatory
     * blocks; a missing block is not checked before advancing past its header. */
    reader->data = (const unsigned char *)(func_00153de8(reader->file) + 1);
    reader->labels = (const struct SequenceLabelTable *)
        (func_00153e54(reader->file) + 1);
    return reader;
invalid:
    return reader;
}
