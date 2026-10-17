#include "game/glyph.h"
float func_00122280(GlyphRenderer *renderer, unsigned short codepoint)
{
    GlyphMetrics glyph;
    float left, advance;
    func_0012f4e4(&glyph);
    renderer->font->getGlyph(&glyph, codepoint);
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
    func_0012f1f4(renderer, &glyph, renderer->x + left);
    renderer->x += advance;
    return advance;
}
