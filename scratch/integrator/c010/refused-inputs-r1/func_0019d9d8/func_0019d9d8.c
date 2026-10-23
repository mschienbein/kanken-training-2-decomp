#include "game/font.h"

extern void func_00113e20(struct FontResource *font);
extern int func_0018f634(struct FontResource *font);

void *func_0019d9d8(struct FontResource *font, void *textureSheets)
{
    void *previous = font->textureSheets;
    if (previous != textureSheets) {
        if (previous != 0)
            func_00113e20(font);
        font->textureSheets = textureSheets;
        if (textureSheets != 0)
            func_0018f634(font);
        return previous;
    }
    return textureSheets;
}
