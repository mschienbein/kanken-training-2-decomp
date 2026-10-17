/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned declaration-only interface, from public NW4C call sites:
 * include/nw/gfx/gfx_FrameBuffer.h and sources/libraries/gfx/gfx_GlImplement.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * [INFERENCE] Scalar signedness/unused return contracts follow those uses, not
 * any Nintendo SDK header. No object layout or implementation is supplied.
 */
#pragma once
#include "../../../GLES2/gl2.h"
extern "C" {
void nngxAddB2LTransferCommand(const GLvoid*, GLsizei, GLsizei, GLenum,
                             GLvoid*, GLsizei, GLsizei, GLenum, GLenum, GLboolean, GLsizei);
void nngxAddVramDmaCommand(const GLvoid*, GLvoid*, GLsizei);
void nngxAddMemoryFillCommand(GLvoid*, GLsizei, GLuint, GLenum,
                             GLvoid*, GLsizei, GLuint, GLenum);
void glLogicOp(GLenum);
}
