#include "game/exp_heap.h"
#include "game/formatted_text.h"
unsigned short func_001f15e0(const struct FormattedText *text)
{
    if (text->buffer_bytes == 0)
        return 0;
    return (unsigned short)((text->buffer_bytes >> 1) - 1);
}
