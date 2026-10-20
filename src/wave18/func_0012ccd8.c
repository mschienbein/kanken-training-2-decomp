#include "game/functions.h"
#include "game/bss_0049ecf0.h"



void func_0012ccd8(struct CommandQueue *detail, struct Command *command)
{
    detail->patch->pair_count = ((unsigned)((const unsigned char *)command->source + command->size -
        (const unsigned char *)detail->patch) - sizeof(struct CommandPatch)) >> 3;
    func_0013776c((void *)command->source, command->size);
    command->flags |= 4;
    command->size = detail->captured_size;
    detail->patch = 0;
    detail->captured_size = 0;
}
