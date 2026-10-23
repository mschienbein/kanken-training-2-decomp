/* SPDX-License-Identifier: GPL-3.0-only
 * Cleanup organization follows the public ResFontBase resource-removal methods:
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Independently adapted to the canonical FontResource interfaces. */
#include "game/font.h"
#include "nw/font/font_GraphicsApi.h"

/* [INFERENCE] Texture-name deletion is the public ResFontBase helper
 * inlined into resource removal; historical TU identity is not claimed. */
static void delete_texture_names(FontResource *font)
{
    FontTexture *sheets = font->textures;
    const int count = font->vtable->texture_count(font);
    for (int i = 0; i < count; ++i) {
        const unsigned texture = sheets[i].handle;
        if (texture != 0) {
            glDeleteTextures(1, &texture);
            sheets[i].handle = 0;
        }
    }
}

extern "C" FontFile *func_0013ea40(FontResource *font)
{
    if (font->textures != 0) {
        delete_texture_names(font);
        font->textures = 0;
    }
    FontFile *file = font->file;
    font->file = 0;
    font->information = 0;
    return file;
}
