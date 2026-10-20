#include "game/animation.h"

struct W11StateScreen {
    unsigned char reserved00[0x4c];
    struct AnimationOwner *display;
    unsigned char reserved50[0x40];
    int mode;
    int previous_mode;
    int states[8];
    int counter;
};

extern void func_001dfcb0(struct W11StateScreen *screen);
extern void func_001de4f8(struct W11StateScreen *screen);
extern void func_001dfe88(struct W11StateScreen *screen);
extern void func_001df9d0(struct W11StateScreen *screen);
extern void func_001de638(struct W11StateScreen *screen);
extern void func_001df028(struct W11StateScreen *screen);
extern void func_001decc0(struct W11StateScreen *screen);
extern void func_001692f8(struct W11StateScreen *screen, int enabled);
extern void func_001dd6c8(struct W11StateScreen *screen);

static __inline void update_modal_state(struct W11StateScreen *screen)
{
    switch (screen->states[6]) {
    case 39:
        func_001692f8(screen, 1);
        screen->states[screen->mode] = 40;
        screen->counter = 0;
        break;
    case 40:
        func_001dd6c8(screen);
        if (func_0017710c(screen->display, 3) == 0) {
            screen->states[screen->mode] = 41;
            screen->counter = 0;
        }
        break;
    case 41: {
        int previous;
        func_001692f8(screen, 0);
        previous = screen->mode;
        screen->mode = screen->previous_mode;
        screen->previous_mode = previous;
        screen->counter = 0;
        break;
    }
    }
}

void func_001debb4(struct W11StateScreen *screen)
{
    switch (screen->mode) {
    case 0: func_001dfcb0(screen); break;
    case 1: func_001de4f8(screen); break;
    case 2: func_001dfe88(screen); break;
    case 3: func_001df9d0(screen); break;
    case 4: func_001de638(screen); break;
    case 5: func_001df028(screen); break;
    case 6: update_modal_state(screen); break;
    case 7: func_001decc0(screen); break;
    }
}
