#include "game/font.h"
#include "game/font_resource.h"



extern "C" FontTexture *func_001db318(FontResource *font, FontTexture *textures)
{
    FontTexture *previous = font->textures;
    if (previous == textures)
        return textures;
    if (previous != 0)
        func_001daea4(font);
    font->textures = textures;
    if (textures != 0)
        func_001dae08(font);
    return previous;
}
