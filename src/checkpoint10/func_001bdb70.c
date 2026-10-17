#include "game/selection.h"

struct W6HandlePanel10;
struct W6DisplayHandle;
/* INTEGRATOR: differs from src/wave6/func_0016c55c.c: index at 0x90 selects two current/previous state slots. */
struct W6SelectionReset {
    unsigned char reserved00[0x4c];
    struct W6ListController *list;
    unsigned char reserved50[0x0c];
    struct W6HandlePanel10 *panel;
    unsigned char reserved60[0x30];
    int stack_index;
    int modes[2];
    int previous_modes[2];
    unsigned char reserveda4[8];
    struct W6DisplayHandle **handles;
};
extern void func_0016c55c(struct W6SelectionReset *state);
extern void func_00181b68(unsigned mask, unsigned direction, int animate);
extern int func_00177124(unsigned mask);

static __forceinline void set_mode(struct W6SelectionReset *screen, int mode)
{
    screen->modes[screen->stack_index] = mode;
}

void func_001bdb70(struct W6SelectionReset *screen)
{
    switch (screen->modes[screen->stack_index]) {
    case 3:
        func_00181b68(7, 1, 1);
        screen->previous_modes[screen->stack_index] = screen->modes[screen->stack_index];
        set_mode(screen, 4);
        break;
    case 4:
        if (func_00177124(7) == 0) {
            func_0016c55c(screen);
            func_00181b68(7, 0, 1);
            screen->previous_modes[screen->stack_index] = screen->modes[screen->stack_index];
            set_mode(screen, 5);
        }
        break;
    case 5: {
        int state = func_00177124(7);
        if (state == 0)
            screen->stack_index = state;
        break;
    }
    }
}
