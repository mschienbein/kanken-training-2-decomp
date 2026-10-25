#include "game/graphics_state.h"

void func_001376fc(void *start0, unsigned value0, void *end0,
                   unsigned width0, void *start1, unsigned value1, void *end1,
                   unsigned width1)
{
    struct GraphicsTransferCommand command;
    {
        const struct GraphicsTransferCommand * const *initial = &data_00243784.command;
        command = **initial;
    }
    command.header.opcode = 2;
    command.header.channel = data_00243784.channel;
    command.parameters[0] = (unsigned)start0;
    command.parameters[1] = (unsigned)end0;
    command.parameters[2] = value0;
    command.parameters[3] = (unsigned)start1;
    command.parameters[4] = (unsigned)end1;
    command.parameters[5] = value1;
    command.final.first = (unsigned short)width0;
    command.final.second = (unsigned short)width1;
    func_00182bb0(&func_001861c4()->commands, &command);
}

