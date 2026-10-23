struct GlyphVertex {
    float x, y;
    unsigned color;
    float u, v;
};

struct GlyphRenderBuffer {
    unsigned reserved00;
    unsigned texture_handle;
    struct GlyphVertex vertices[4];
    unsigned char reserved58[0x84];
    const void *texture;
    float parameter;
};

#include "game/command_buffer.h"

void func_00141554(struct GlyphRenderBuffer *buffer, float value)
{
    buffer->parameter = value;
    func_00141560(1, value);
}
