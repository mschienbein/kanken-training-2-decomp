/* SPDX-License-Identifier: GPL-3.0-or-later
 * Independently authored API declarations used by the public NW4C font source.
 * No SDK or OpenGL implementation is included.
 * glDeleteTextures: https://registry.khronos.org/OpenGL-Refpages/es2.0/xhtml/glDeleteTextures.xml
 * The buffer-update return type follows the existing matching wrapper; its
 * readonly input follows the public NW4C caller. The binding is relocation-proved.
 */
#pragma once
#include <nw/types.h>
typedef unsigned int GLuint;
extern "C" {
void glDeleteTextures(int count, const GLuint* textures);
/* [INFERENCE] Readonly input follows the public NW4C callers. The int return
 * retains the established matching wrapper contract at the same entry. */
int nngxUpdateBuffer(const void* buffer, u32 size);
}
