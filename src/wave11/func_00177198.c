#include "game/messages.h"
void func_00177198(struct MessageLayout *display, const struct W11TextResource *resource, int index)
{
    if (index == -1) {
        func_0017a43c(display, data_002276bc, 0, 0);
    } else {
        const unsigned short *attributes = func_001dcbd4(resource, index);
        const unsigned short *text = func_00170c00(resource, index);
        func_0017a43c(display, text, attributes, 0);
    }
}
