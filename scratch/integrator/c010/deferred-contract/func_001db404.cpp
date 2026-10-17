/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/font/font_ResFont.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: express the located non-deleting cleanup with an explicit typed vtable view.
 * This file is free software under GNU GPL version 3; see LICENSE.
 */
struct ResourceFontVTable;
struct ResourceFont {
    const ResourceFontVTable *vtable;
    void *resource;
};

/* INTEGRATOR: move to include/game/resource_font.h */
extern "C" {
extern const ResourceFontVTable data_00224d30;
extern void *func_001daefc(ResourceFont *font);
extern void func_001db130(void *object);
void func_001db404(ResourceFont *font);
}

void func_001db404(ResourceFont *font)
{
    void *resource = font->resource;
    font->vtable = &data_00224d30;
    if (resource)
        func_001daefc(font);
    func_001db130(font);
}
