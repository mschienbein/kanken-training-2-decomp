#include "game/resource_font.h"
/* [INFERENCE] Partial NW4C text-box state and its allocation callback. */
struct LayoutFont;
struct LayoutTextBox;

struct LayoutTextBoxVTable {
    const void *reserved00[28];
    void (*allocate_string)(LayoutTextBox *, unsigned short length, unsigned int format);
};
struct LayoutTextBox {
    const LayoutTextBoxVTable *vtable;
    unsigned char reserved04[0xd0];
    unsigned short *text;
    LayoutColor colors[2];
    LayoutFont *font;
    float width, height;
    float line_spacing, character_spacing;
    void *tag_processor;
    unsigned short buffer_bytes;
    unsigned short text_length;
    unsigned char position, alignment, dirty, reservedff;
    void *material;
    void *display_buffer;
};

extern "C" void func_001cf7b0(LayoutTextBox *object, unsigned short length, unsigned int format)
{
    object->text = 0;
    object->buffer_bytes = 0;
    object->text_length = 0;
    object->font = 0;
    object->width = 0.0f;
    object->height = 0.0f;
    object->position = (unsigned char)(object->position / 3 * 3 + 1);
    object->position = (unsigned char)(object->position % 3 + 3);
    object->line_spacing = 0.0f;
    object->character_spacing = 0.0f;
    object->tag_processor = 0;
    object->display_buffer = 0;
    object->alignment = 0;
    object->dirty = 0;
    object->vtable->allocate_string(object, length, format);
}
