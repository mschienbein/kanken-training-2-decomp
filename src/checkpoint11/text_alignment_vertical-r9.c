#include "game/screen.h"


void func_00180388(struct DisplayObject *object, unsigned char alignment)
{
    struct TrainingWidget *widget = (struct TrainingWidget *)object;
    struct PackedTextState *text = widget->bindings[0]->text;
    unsigned char flags = text->alignment;
    unsigned char origin = (flags & 15) % 3u;
    unsigned char selection = alignment * 3 + origin;
    text->alignment = (flags & 0xf0) | selection;
}
