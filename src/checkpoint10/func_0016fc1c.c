#include "game/playback_screen.h"
#include "game/screen_transition.h"
#include "game/selection.h"



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
