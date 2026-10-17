#include "game/item_text.h"

class W12cFont {
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
    virtual void get_glyph(W12cGlyphMetrics *glyph, unsigned short code);
};

struct W12cGlyphWriter {
    unsigned char reserved00[0x24];
    float scale_x;
    float scale_y;
    float cursor_x;
    unsigned char reserved30[8];
    float fixed_advance;
    W12cFont *font;
    unsigned char reserved40[8];
    unsigned char fixed_width;
};

/* INTEGRATOR: move to include/game/font.h */
extern "C" {
void func_001d9e0c(W12cGlyphWriter *writer, const W12cGlyphMetrics *metrics, float x);
}

extern "C" float func_001da39c(W12cGlyphWriter *renderer, unsigned short codepoint)
{
    W12cGlyphMetrics glyph;
    float left, advance;
    func_001db138(&glyph);
    renderer->font->get_glyph(&glyph, codepoint);
    if (renderer->fixed_width) {
        advance = renderer->fixed_advance;
        left = advance;
        left -= (float)glyph.advance * renderer->scale_x;
        left *= 0.5f;
        left += (float)glyph.left * renderer->scale_x;
    } else {
        const int left_pixels = glyph.left;
        const int advance_pixels = glyph.advance;
        const float scale = renderer->scale_x;
        const float advance_width = (float)advance_pixels;
        const float left_width = (float)left_pixels;
        advance = advance_width * scale;
        left = left_width * scale;
    }
    func_001d9e0c(renderer, &glyph, renderer->cursor_x + left);
    renderer->cursor_x += advance;
    return advance;
}
