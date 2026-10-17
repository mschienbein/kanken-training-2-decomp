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
/* INTEGRATOR: maps to src/wave6/func_0018f6c4.c, currently int(void*, unsigned).
 * Keep its int return; reconcile that wrapper and func_0012f570's buffer parameter
 * with const void* after reviewing the cache-maintenance implementation. */
int nngxUpdateBuffer(const void* buffer, u32 size);
}
