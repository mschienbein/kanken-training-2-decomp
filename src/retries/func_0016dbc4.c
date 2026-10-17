#include <string.h>

#include "game/screen.h"



void func_0016dbc4(struct W10DrawingInput *input)
{
    int active = 0;
    memset(input->characters, 0, sizeof(input->characters));
    input->field89a = active;
    input->count = active;
    input->progress = 0.0f;
    func_0019cecc(input->buffer, sizeof(input->buffer));
    input->status = active;
    active = 1;
    input->active = active;
    func_00180344();
    func_001826fc(input->display, 1);
    func_00166d5c(input, 0, 0.0f, 0.0f);
    input->state = active;
    input->finished = active;
}
