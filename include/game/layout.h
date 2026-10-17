#ifndef GAME_LAYOUT_H
#define GAME_LAYOUT_H

#include "game/matrix.h"

struct PaneStyle;
struct Pane {
    unsigned char reserved00[5];
    unsigned char alignment;
    unsigned char reserved06[0x12];
    int extent;
    unsigned char reserved1c[0x4c];
    struct PaneStyle *style;
};

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
extern void func_001c5930(struct PaneStyle *style, int extent);
extern void func_001c59fc(struct Pane *pane, unsigned alignment);
#ifdef __cplusplus
}
#endif

#endif
