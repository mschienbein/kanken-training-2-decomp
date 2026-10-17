/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr, include/nw/version.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select the upstream Garden (3.7.4) preset.
 */
#pragma once
#define VER_STR_(version) #version
#define VER_STR(version) VER_STR_(version)
#define NW_VERSION_NONE 0
#define NW_VERSION_MILLI4C 1
#define NW_VERSION_REDPEPPER 2
#define NW_VERSION_CTRDASH 3
#define NW_VERSION_GARDEN 4
#define NW_VERSION_STICKSTR 5
#define NW_VERSION NW_VERSION_GARDEN
#define NW_VERSION_MAJOR 3
#define NW_VERSION_MINOR 7
#define NW_VERSION_MICRO 4
#define NW_VERSION_NUMBER(major, minor, micro, id) (((major) << 24) | ((minor) << 16) | ((micro) << 8) | ((id) << 0))
#define NW_CURRENT_VERSION_NUMBER "NW4C_" VER_STR(NW_VERSION_MAJOR) "_" VER_STR(NW_VERSION_MINOR) "_" VER_STR(NW_VERSION_MICRO)
