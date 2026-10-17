#include "game/functions.h"
#include "game/service_state.h"

void func_001865ec(int first, int second)
{
    struct WorkCommand *command = &bss_004990d0.commands[bss_004990d0.writeIndex];
    bss_004990d0.writeIndex = (bss_004990d0.writeIndex + 1) % 256;
    command->operation = 2;
    command->flags = 0;
    command->first = first;
    command->second = second;
}
