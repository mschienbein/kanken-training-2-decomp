#include "game/input.h"
void func_00123a64(struct ButtonSample *sample)
{
    func_00123a70(&sample->held, &sample->pressed, &sample->released);
}
