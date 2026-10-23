#include "game/event_route.h"
unsigned char *DrawCommandState::commit_to(unsigned char *commands, unsigned flags)
{
    field698 = 0;
    secondary_components = 0;
    mode = 0;
    kind = 6;
    this->flags = 1;
    commands = func_0016b27c(this, commands, 0, 0);
    return InputDeviceState::commit_to(commands, flags);
}
