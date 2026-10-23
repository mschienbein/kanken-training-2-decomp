/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned minimal DMP constant declaration; no SDK header copied.
 * [INFERENCE] Public NW4C gfx_RenderContext.h::ToLutTexture supplies the name.
 * The game's texture-target dispatch in func_00184808 normalizes the LUT
 * selector by subtracting 0x6600, with separate cases at 0x6616 and 0x661f.
 * This token is absent from the public Khronos registry subset. See
 * src/nw4c/FACTS-HEADER-GAPS.json for callers and evidence limitations.
 */
#pragma once
#define GL_LUT_TEXTURE0_DMP 0x6600
