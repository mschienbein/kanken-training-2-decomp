#ifndef GAME_DISPLAY_INITIALIZER_H
#define GAME_DISPLAY_INITIALIZER_H

#include <stdint.h>
#include "game/screen.h"

#ifdef __cplusplus
extern "C" {
#endif
struct DisplayObject *func_00181588(struct DisplayObject *object, int index,
                                  int capacity, int count);
void func_00181520(struct DisplayObject *object, int index, uintptr_t archive,
                   int archive_index, const char *layout, const char *animation);
void func_001814e8(struct DisplayObject *object, int index, uintptr_t archive,
                   int archive_index, const char *name);
/* [INFERENCE] A slot-shaped screen setter may have been inlined and removed.
 * The caller supplies index, although this partial receiver stores one screen.
 * The helper's historical identity, outlining and TU membership are unrecovered. */
void w16_set_screen(struct BoundDisplayObject *object, int index, int screen);
void func_001456e4(void);
#ifdef __cplusplus
}
#endif

#endif
