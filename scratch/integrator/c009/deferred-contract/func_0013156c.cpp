#include "game/text.h"

void func_0013156c(CodeUnitReader *reader, EncodingSource *source)
{
    CodeUnitReader::NextCodeUnit next = 0;
    if (source->vtable->encoding(source) == 1)
        next = data_0021c5e0[3];
    reader->cursor.pointer = 0;
    reader->next = next;
}
