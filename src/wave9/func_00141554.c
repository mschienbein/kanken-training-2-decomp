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

extern void func_00141560(unsigned mode, float value);

void func_00141554(struct GlyphRenderBuffer *buffer, float value)
{
    buffer->parameter = value;
    func_00141560(1, value);
}
