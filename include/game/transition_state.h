#ifndef GAME_TRANSITION_STATE_H
#define GAME_TRANSITION_STATE_H

struct DisplayObject;
struct TransitionDisplayLink {
    unsigned reserved00;
    struct DisplayObject *object;
};
/* The resetter indexes 0x24-byte records; the fade reader selects their
 * +0x1c previous values at absolute offsets +0x1c and +0x40. */
struct TransitionDisplay {
    unsigned reserved00;
    unsigned state;
    unsigned reserved08;
    float elapsed;
    unsigned char reserved10[12];
    float previous;
    struct TransitionDisplayLink *link;
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct TransitionDisplay bss_0028d95c[];
#ifdef __cplusplus
}
#endif
#endif
