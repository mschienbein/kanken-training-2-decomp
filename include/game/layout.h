#ifndef GAME_LAYOUT_H
#define GAME_LAYOUT_H

#ifdef __cplusplus
namespace nw { namespace lyt {
class Layout;
class AnimTransform;
class ResourceAccessor;
} }
/* [INFERENCE] The original pointer-result slot matches native Layout's
 * AnimResource overload; no placeholder virtual owner or erased return. */
extern "C" nw::lyt::AnimTransform *func_001ce8b8(nw::lyt::Layout *layout,
    const void *file, nw::lyt::ResourceAccessor *accessor);
#endif
#include "game/matrix.h"

#include "game/audio_voice.h"

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
extern void func_001c5930(struct AudioChannelState *style, int extent);
extern void func_001c59fc(struct AudioChannel *channel, unsigned alignment);
extern void func_001c5a08(struct AudioChannelState *style, unsigned alignment);
#ifdef __cplusplus
}
#endif

#endif
