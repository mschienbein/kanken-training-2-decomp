#ifndef GAME_WIDGET_ANIMATION_H
#define GAME_WIDGET_ANIMATION_H

struct WidgetAnimationTrack {
    int state;
    unsigned flags;
    unsigned char reserved08[8];
    int result;
    unsigned char reserved14[0x20];
    int present;
    unsigned char reserved38[0x1c];
};
struct AnimationBank {
    unsigned char reserved00[0x18];
    #ifdef __cplusplus
    struct WidgetAnimationTrack tracks[1];
#else
    struct WidgetAnimationTrack tracks[];
#endif
};

#endif
