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

void func_001360ec(int index, int value)
{
    bss_0049c878.items[index].value = value;
}
