#include "game/playback_screen.h"
#include "game/screen_transition.h"
#include "game/selection.h"

/* INTEGRATOR: differs from src/wave10/func_001b06a4.c: the same object has
 * transition state fields at +0x90 through +0xa4; retain its established prefix. */
struct SettingsScreen {
    unsigned char reserved00[0x34];
    int next_state;
    unsigned char reserved38[0x14];
    struct W6ListController *controls;
    unsigned char reserved50[0x40];
    int state;
    unsigned char reserved94[8];
    int pending_state;
    int previous_state;
    int transition;
};


void func_0016fc1c(struct SettingsScreen *screen, int state, int flags, int transition)
{
    int sound = -1;
    switch (transition) {
    case 1: sound = 7; break;
    case 2: sound = 3; break;
    case 3: sound = 4; break;
    }
    if (flags & 1) {
        if (sound == -1) {
            screen->previous_state = screen->state;
            screen->state = state;
        } else {
            screen->state = 10;
            screen->pending_state = state;
        }
        screen->transition = transition;
        return;
    }
    if (sound != -1)
        func_00181b68(sound, flags, 1);
    screen->previous_state = screen->state;
    screen->state = state;
}
