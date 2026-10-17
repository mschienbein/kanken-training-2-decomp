#ifndef GAME_LAYOUT_H
#define GAME_LAYOUT_H

#include "game/matrix.h"

struct LayoutAnimationTransform;
struct LayoutAnimationLink;
struct LayoutDrawInfo;
struct LayoutRenderContext;

struct LayoutListNode {
    struct LayoutListNode *next;
    struct LayoutListNode *previous;
};

struct LayoutAnimationList {
    unsigned size;
    struct LayoutListNode sentinel;
};

struct LayoutMaterial {
    const void *vtable;
    struct LayoutAnimationList animation_links;
};

struct LayoutPane {
    unsigned char reserved00[0x1c];
    struct LayoutAnimationList animation_links;
};

struct LayoutMatrixSource {
    unsigned char reserved00[0x8c];
    struct Matrix34 matrix;
};

struct LayoutAnimationStatus {
    unsigned char reserved00[0x10];
    unsigned flags;
};

#ifdef __cplusplus
extern "C" {
#endif
extern void func_0016b5fc(struct LayoutAnimationList *list,
                          const struct LayoutAnimationTransform *animation);
extern struct LayoutAnimationLink *func_001cff58(
    const struct LayoutAnimationList *list,
    const struct LayoutAnimationTransform *animation);
#ifdef __cplusplus
}
#endif

#endif
