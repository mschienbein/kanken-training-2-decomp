#include "game/sound_format.h"

int func_001f2404(const struct SequenceReader *reader,
                  const char *name, unsigned *offset)
{
    return func_001f240c(reader->labels, name, offset);
}
