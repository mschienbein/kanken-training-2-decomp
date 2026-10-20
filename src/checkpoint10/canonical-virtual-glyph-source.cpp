#include "game/item_text.h"

#include "game/font.h"

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
