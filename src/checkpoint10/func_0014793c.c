#include "game/screen.h"
struct TransitionDisplayLink {
    unsigned reserved00;
    struct DisplayObject *object;
};
struct TransitionDisplay {
    unsigned reserved00;
    unsigned state;
    unsigned reserved08;
    float elapsed;
    unsigned char reserved10[12];
    float previous;
    struct TransitionDisplayLink *link;
};
extern struct TransitionDisplay bss_0028d95c[];

static void reset_transition(int index, float previous)
{
    bss_0028d95c[index].state = 0;
    bss_0028d95c[index].elapsed = 0.0f;
    bss_0028d95c[index].previous = previous;
}

void func_0014793c(int index)
{
    reset_transition(index, 0.0f);
    if (bss_0028d95c[index].link) {
        func_0018271c(bss_0028d95c[index].link->object, 0);
        func_001826fc(bss_0028d95c[index].link->object, 0);
    }
}
