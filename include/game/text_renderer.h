#ifndef GAME_TEXT_RENDERER_H
#define GAME_TEXT_RENDERER_H

struct RecordSet;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
extern "C" {
#endif
extern void func_00173748(int secondary, int category, int index, int *value, int *extra);
extern void func_001dbcb0(int category, int kind, int index, int *value);
extern int func_001e33dc(const struct RecordSet *set, int index);
extern const void *func_001e33f8(const struct RecordSet *set, int index);
extern const unsigned short *func_001e3468(const struct RecordSet *set, int index);
extern int func_001e3490(const struct RecordSet *set, int index);
extern const void *func_001e34ac(const struct RecordSet *set, int index);
extern const void *func_001e34d4(const struct RecordSet *set, int index);
extern const void *func_001e34fc(const struct RecordSet *set, int index);
extern const unsigned short *func_001e3524(const struct RecordSet *set, int index);
#ifdef __cplusplus
}
#endif

#endif
