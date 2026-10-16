#include "game/text.h"

int func_0017adc4(const unsigned short *text)
{
    int length = 0;
next_character:
    if (*text == 0)
        return length;
    ++length;
    ++text;
    goto next_character;
}
