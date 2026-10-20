#include "game/text.h"

void func_001f4cf8(CodeUnitReaderAlt *reader, EncodingSource *source)
{
    CodeUnitReaderAlt::NextCodeUnit next = 0;
    if (source->vtable->encoding(source) == 1)
        next = data_0021c64c[3];
    reader->cursor.pointer = 0;
    reader->next = next;
}
