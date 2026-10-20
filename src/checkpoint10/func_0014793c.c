#include "game/screen.h"
#include "game/transition_state.h"

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
