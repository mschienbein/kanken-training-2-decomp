#include "game/functions.h"
/* Shared declaration copied exactly from src/pilot/loop_state.c. */
struct Item { int value; int flags; int id; };
struct State {
    int a, b, c;
    unsigned char enabled, active, ready;
    char reserved;
    int position;
    int step;
    struct Item items[8];
    int count;
};
extern struct State bss_0049c878;

void func_0013608c(unsigned char enabled, unsigned char active, unsigned char ready)
{
    bss_0049c878.enabled = enabled;
    bss_0049c878.active = active;
    bss_0049c878.ready = ready;
}
