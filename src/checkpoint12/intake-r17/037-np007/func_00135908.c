#include "game/scene_animation.h"

struct FrameAdvance { float count; float speed; float step; };

void func_00135908(struct AnimationObject *object, int index)
{
    struct SceneAnimationTrack *track = object->tracks[index];
    float frame;
    float length;
    struct FrameAdvance advance;

    if (!track->resource)
        return;
    track->flags &= ~0x10u;
    if (!(track->flags & 1))
        return;

    frame = track->resource->frame;
    length = (float)func_001409c0(track->resource);
    advance.count = (float)data_0023fdc8;
    advance.speed = track->speed;
    advance.step = advance.speed * bss_00495b30.nominalFrames;
    frame += advance.count * advance.step;
    if (frame < 0.0f) {
        frame += length;
        track->flags |= 0x10;
    } else if (frame >= length) {
        frame -= length;
        track->flags |= 0x10;
    }

    switch (track->mode) {
    case 1:
        if (advance.speed < 0.0f) {
            if (frame < track->limit)
                frame = track->limit;
        } else if (advance.speed > 0.0f) {
            if (frame > track->limit)
                frame = track->limit;
        }
        break;
    case 2:
        if (track->flags & 0x10)
            frame = track->frame;
        break;
    }
    track->frame = frame;
    track->resource->frame = frame;
}
