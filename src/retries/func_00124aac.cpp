#include "game/stream_manager.h"




extern "C" void func_00124aac(struct RenderResource *resource)
{
    struct StreamManager *arena = func_001916e8();
    ResourceCommand *command = (ResourceCommand *)func_0019147c(arena, 5);
    command->start = arena->write_index;
    func_00191458(arena, command->initialize(resource));
    unsigned serial = func_001912b0(arena, 1);
    func_0019124c(arena, serial);
    resource->count = 0;
    resource->allocation = 0;
}
