struct TextureCache {
    unsigned handle;
};

struct GlyphMetrics {
    const void *texture;
    signed char left;
    unsigned char width;
    signed char advance;
    unsigned char height;
    unsigned short texture_width;
    unsigned short texture_height;
    unsigned short texture_x;
    unsigned short texture_y;
    unsigned char dirty;
    unsigned char reserved11;
    unsigned short format;
    struct TextureCache *cache;
};

void func_0012f4e4(struct GlyphMetrics *glyph)
{
    glyph->dirty = 0;
}
