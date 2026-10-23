#ifndef GAME_TEXT_WRITER_H
#define GAME_TEXT_WRITER_H

#include "game/font.h"

#ifdef __cplusplus
#include <preinclude.h>
#include <nw/font/font_WideTextWriter.h>

/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct TextWriterState;
struct TextBounds;

struct TextWriterState {
    unsigned char reserved00[0x28];
    float scale_y;
    float cursor_x;
    float cursor_y;
    unsigned reserved34[2];
    struct FontResource *font;
    unsigned char reserved40[0x1c];
    unsigned flags;
    unsigned reserved60;
};
struct TextBounds {
    float left, top, right, bottom;
};

extern "C" {
extern void func_00192720(void *destination, const void *source, unsigned size);
extern unsigned func_00186afc(struct TextWriterState *writer, struct TextBounds *bounds,
                          const unsigned short **cursor, int length);
extern void func_00174480(void *object);
extern unsigned func_0016471c(struct TextWriterState *writer, struct TextBounds *bounds,
                          const unsigned short **cursor, int length);
}
#endif

#endif
