#include "game/layout.h"

/* [INFERENCE] Vtable slot 0x68: func_00117ea8 forwards the command-buffer
 * cursor, draw-info pointer and renderer; the base pane emits no commands. */
extern "C" unsigned char *func_001f01b0(LayoutPane *pane,
                                       unsigned char *cursor,
                                       LayoutDrawInfo *draw_info,
                                       LayoutRenderContext *renderer)
{
    return cursor;
}
