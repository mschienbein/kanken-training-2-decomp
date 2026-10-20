#ifndef GAME_TEXT_WRITER_H
#define GAME_TEXT_WRITER_H

#include <preinclude.h>
#include <nw/font/font_WideTextWriter.h>

/* The matching getter bodies use the native font vtable and writer fields.
 * This is their actual const receiver, not an alias of a padded W20 view. */
extern "C" {
float func_0017a690(const nw::font::WideTextWriter *writer);
float func_0017a6c8(const nw::font::WideTextWriter *writer);
}

#endif
