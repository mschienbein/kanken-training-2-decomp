#include "game/command_group.h"


extern "C" void func_00151458(CommandGroup *group, unsigned mask, signed char value)
{
    int i = 0;
    void (CommandTarget::*callback)(signed char) = data_0021df3c.flagf4;
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
