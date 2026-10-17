#ifndef GAME_RENDER_PARAMETER_H
#define GAME_RENDER_PARAMETER_H

#include <stddef.h>
#include "game/runtime.h"

struct RenderParameter {
    unsigned int kind;
    float x, y;
    unsigned char red, green, blue, alpha;
#ifdef __cplusplus
    RenderParameter();
    RenderParameter(float x, float y, unsigned int red, unsigned int green,
                    unsigned int blue, unsigned int alpha);
#endif
};

#ifdef __cplusplus
void *operator new(size_t size, const void *tag) throw();
extern "C" {
#endif
extern struct RenderParameter *bss_00291104[3];
extern struct RenderParameter *bss_00291110[3];
extern struct RenderParameter *bss_0029111c[4];
extern struct RenderParameter *bss_00291204[7];
extern struct RenderParameter *bss_00291220[4];
extern struct RenderParameter *bss_00291230[5];
extern void func_001f4fb0(void);
extern void func_002059b8(void);
extern void func_002066ac(void);
extern void func_00206848(void);
extern void func_00212434(void);
extern void func_00212598(void);
#ifdef __cplusplus
}
#endif

#endif
