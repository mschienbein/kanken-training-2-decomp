/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/font.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "font/font_CharStrmReader.h"
#include "font/font_Font.h"
#include "font/font_ResFont.h"

#include "font/font_CharWriter.h"
#include "font/font_TextWriterBase.h"
#include "font/font_TagProcessorBase.h"

#include "font/font_WideTextWriter.h"

#ifdef __cplusplus

using namespace nw::font;
using namespace nw::font::internal;

#endif

