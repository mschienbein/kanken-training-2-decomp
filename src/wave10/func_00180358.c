#include "game/screen.h"

extern void func_001803d0(struct DisplayObject *widget, unsigned char alignment);
extern void func_00180388(struct DisplayObject *widget, unsigned char alignment);

void func_00180358(struct DisplayObject *widget, int horizontal, int vertical)
{
    unsigned char x = horizontal + 1;
    unsigned char y = vertical + 1;
    func_001803d0(widget, x);
    func_00180388(widget, y);
}
