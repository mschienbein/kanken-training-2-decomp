#include "game/screen.h"

struct W10DrawingInput {
    unsigned char reserved00[0xc];
    struct DisplayObject *display;
    unsigned char reserved10[0xa];
    unsigned short characters[0x440];
    short field89a;
    short count;
    unsigned char reserved89e[0x1a];
    unsigned char active;
    unsigned char finished;
    unsigned char reserved8ba[6];
    float progress;
    int state;
    unsigned char reserved8c8[0x14];
    unsigned char buffer[0x800];
    unsigned int reserved10dc;
    int status;
};

unsigned short func_0016d2dc(const struct W10DrawingInput *input)
{
    unsigned short result = 0;
    if (input->count > 0)
        result = input->characters[0];
    return result;
}
