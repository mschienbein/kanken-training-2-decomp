#ifndef GAME_SCENE_ANIMATION_H
#define GAME_SCENE_ANIMATION_H

#include "game/animation.h"

/* np007 and the accepted func_001409c0 agree on this resource prefix.
 * [INFERENCE] Scene object/track field names describe the observed offsets;
 * no historical object or translation-unit identity is asserted. */
struct AnimationHeader {
    unsigned char reserved00[8];
    unsigned short frame_count;
};
struct AnimationResource {
    unsigned char reserved00[12];
    const struct AnimationHeader *header;
    float frame;
};
struct AnimationObject;
struct SceneAnimationTrack {
    unsigned reserved00;
    unsigned flags;
    float frame;
    float speed;
    unsigned reserved10;
    int mode;
    float limit;
    /* This unused callback word has no adopted callable contract here. */
    unsigned reserved1c;
    struct AnimationResource *resource;
};
struct SceneVector {
    float x, y, z;
};
struct AnimationObject {
    struct SceneVector position;
    struct SceneVector rotation;
    struct SceneVector scale;
    float axis_angle;
    struct SceneVector axis;
    unsigned char reserved34[0x58];
    unsigned char transform_order[3];
    unsigned char rotation_order[3];
    unsigned char kind;
    unsigned char reserved93;
    struct SceneAnimationTrack *tracks[3];
    void *active;
    unsigned char reserveda4[0x28];
};

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned func_001409c0(const struct AnimationResource *resource);
extern void func_00135908(struct AnimationObject *object, int index);
#ifdef __cplusplus
}
#endif

#endif
