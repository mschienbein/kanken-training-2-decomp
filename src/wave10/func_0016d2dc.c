#include "game/screen.h"

unsigned short func_0016d2dc(const struct W10DrawingInput *input)
{
    unsigned short result = 0;
    if (input->count > 0)
        result = input->characters[0];
    return result;
}
