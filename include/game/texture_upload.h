#ifndef GAME_TEXTURE_UPLOAD_H
#define GAME_TEXTURE_UPLOAD_H
#include "game/functions.h"
#ifdef __cplusplus
extern "C" {
#endif
void func_00182dc8(unsigned target, int level, unsigned internal_format,
    int width, int height, int border, unsigned format, unsigned type,
    const void *pixels);
void func_00182d9c(unsigned target, unsigned parameter, float value);
void func_00137154(int width, int height, unsigned kind, const void *pixels,
    int linear_minimum, int linear_maximum);
#ifdef __cplusplus
}
#endif
#endif
