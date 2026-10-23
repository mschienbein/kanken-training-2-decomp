#include "game/screen.h"


static unsigned char preserve_vertical(unsigned char flags)
{
    return (flags & 15) / 3 * 3;
}

void func_001803d0(struct DisplayObject *object, unsigned char alignment)
{
    struct TrainingWidget *widget = (struct TrainingWidget *)object;
    struct PackedTextState *text = widget->bindings[0]->text;
    unsigned char flags = text->alignment;
    unsigned char selection = preserve_vertical(flags) + alignment;
    text->alignment = (flags & 0xf0) | selection;
}
