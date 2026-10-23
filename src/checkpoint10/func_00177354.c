#include "game/messages.h"


static void format_label(unsigned short *text, int character)
{
    text[0] = '\n';
    text[1] = 0x3000;
    text[2] = 0x3000;
    text[3] = 'H';
    text[4] = character;
    text[5] = 'H';
    text[6] = 0;
}

void func_00177354(struct MessageLayout *layout, int character)
{
    unsigned short text[7];
    if (character == 0)
        return;
    func_00181aac(2);
    format_label(text, character);
    func_0017a43c(layout, text, 0, 1);
    func_00170dc0(layout, 1, 1);
}
