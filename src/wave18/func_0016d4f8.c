#include "game/screen.h"

extern void func_001eb1f8(struct W10DrawingInput *input);


void func_0016d4f8(struct W10DrawingInput *input)
{
    int i;
    input->active = 0;
    if (input->state == 2 || input->state == 3)
        func_001eb1f8(input);
    func_00180344();
    for (i = 0; i < 3; ++i) {
        if (input->displays[i] != 0)
            func_001826fc(input->displays[i], 0);
    }
    input->finished = 0;
    input->state = 4;
}
