#include "game/command_group.h"


extern "C" void func_0015121c(CommandGroup *group, unsigned mask, unsigned char value)
{
    int i = 0;
    void (CommandTarget::*callback)(unsigned char) = data_0021df3c.mode;
    for (; i < 16; ++i) {
        if (mask == 0)
            break;
        if (mask & 1) {
            CommandTarget *target = group->child(i);
            if (target == 0) {
                mask >>= 1;
                continue;
            }
            (target->*callback)(value);
        }
        mask >>= 1;
    }
}
