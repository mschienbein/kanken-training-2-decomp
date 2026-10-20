#include "game/text_writer.h"
#include "game/font.h"

extern "C" float func_00131540(const TextWriterState *writer)
{
    return (float)writer->font->vtable->ascent(writer->font) * writer->scale_y;
}
