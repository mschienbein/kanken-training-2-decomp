#ifndef GAME_GLYPH_RECORDS_H
#define GAME_GLYPH_RECORDS_H

struct W14GlyphList;
struct W14GlyphObject;
struct W14GlyphState;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
#ifdef __cplusplus
struct W14GlyphObject {
    void *vtable;
    unsigned reserved04[(0x238 - sizeof(void *)) / sizeof(unsigned)];
};
#endif

struct W14GlyphState { int values[4]; };

struct W14GlyphList {
    void *vtable;
    short group;
    signed char kind;
    signed char configured;
    unsigned reserved08;
    int count;
    struct W14GlyphObject **objects;
    signed char limit;
    unsigned char reserved15[3];
    struct W14GlyphState **states;
    signed char *enabled;
};

#ifdef __cplusplus
extern "C" {
#endif
extern void *func_0019d788(unsigned size);
extern struct W14GlyphObject *func_001eb6bc(struct W14GlyphObject *object);
extern void func_001677a4(struct W14GlyphList *list, int group, int kind, int count);
#ifdef __cplusplus
}
#endif

#endif
