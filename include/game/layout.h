#ifndef GAME_LAYOUT_H
#define GAME_LAYOUT_H

#ifdef __cplusplus
#include <preinclude.h>
#include <nw/lyt/lyt_Animation.h>
#endif
#include "game/matrix.h"

#include "game/audio_voice.h"

struct LayoutDrawInfo;
struct LayoutRenderContext;

struct LayoutListNode {
    struct LayoutListNode *next;
    struct LayoutListNode *previous;
};

#ifdef __cplusplus
/* [INFERENCE] These target owner prefixes remain distinct from native Pane and
 * Material. Their animation members use the existing native list contract. */
struct LayoutMaterial {
    const void *vtable;
    nw::lyt::AnimationList animation_links;
};

struct LayoutPane {
    unsigned char reserved00[0x1c];
    nw::lyt::AnimationList animation_links;
};

/* The unbind endpoint retains its address identity: no native name is inferred
 * from its callers. It erases and resets the same native AnimationLink nodes. */
extern "C" void func_0016b5fc(nw::lyt::AnimationList *list,
                              nw::lyt::AnimTransform *animation);
extern "C" void func_001cfd94(LayoutMaterial *material,
                              nw::lyt::AnimTransform *animation);
extern "C" nw::lyt::AnimationLink *func_001cfdac(
    LayoutMaterial *material, nw::lyt::AnimTransform *animation);
extern "C" nw::lyt::AnimationLink *func_001cff50(
    LayoutPane *pane, nw::lyt::AnimTransform *animation);
#else
struct LayoutMaterial;
struct LayoutPane;
#endif

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
extern void func_001c5930(struct AudioChannelState *style, int extent);
extern void func_001c59fc(struct AudioChannel *channel, unsigned alignment);
extern void func_001c5a08(struct AudioChannelState *style, unsigned alignment);
#ifdef __cplusplus
}
#endif

#endif
