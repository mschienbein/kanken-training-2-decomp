#include "game/entry_command.h"

inline void W11EntryCommand::Reset()
{
    resource = -1;
    kind = -1;
    mode = -1;
    option = -1;
    first = 0;
    second = 0;
    active = 0;
    count = 0;
    capacity = 0;
}

W11EntryCommand::W11EntryCommand()
{
    Reset();
}
