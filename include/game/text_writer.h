#ifndef GAME_TEXT_WRITER_H
#define GAME_TEXT_WRITER_H

#include "game/font.h"
struct TextWriterState;
#ifdef __cplusplus
extern "C" {
#endif
void finish_text_writer_copy(struct TextWriterState *writer);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
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
extern unsigned func_0016471c(struct TextWriterState *writer, struct TextBounds *bounds,
                          const unsigned short **cursor, int length);
}
#endif

#endif
