#ifndef GAME_GLYPH_H
#define GAME_GLYPH_H

struct GlyphBatch;
struct GlyphBatchRecord;
struct GlyphFont;
struct GlyphMetrics;
struct GlyphRenderBuffer;
struct GlyphRenderer;
struct GlyphVertex;
struct TextureCache;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
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

struct GlyphBatchRecord {
    float width, height, x, y;
    unsigned color0, color1;
    float u0, v0, u1, v1;
    struct TextureCache *cache;
};

struct GlyphBatch {
    unsigned capacity;
    unsigned short count;
    unsigned char reserved06[0x1a];
};

#ifdef __cplusplus
class GlyphFont {
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void getGlyph(GlyphMetrics *glyph, unsigned short codepoint);
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual unsigned getFilter();
    virtual unsigned getWrap();
};
#endif

struct GlyphRenderer {
    unsigned char reserved00[8];
    unsigned colors[4];
    unsigned batch_color0, batch_color1;
    unsigned reserved20;
    float scale_x, scale_y;
    float x, y;
    float parameter;
    float fixed_advance;
    struct GlyphFont *font;
    struct GlyphRenderBuffer *buffer;
    struct GlyphBatch *batch;
    unsigned char fixed_width;
};

#ifdef __cplusplus
extern "C" {
#endif
void func_0012f4e4(struct GlyphMetrics *glyph);
void func_0012f1f4(struct GlyphRenderer *renderer, const struct GlyphMetrics *glyph, float x);
extern float func_00122280(struct GlyphRenderer *renderer, unsigned short codepoint);
#ifdef __cplusplus
}
#endif

#endif
