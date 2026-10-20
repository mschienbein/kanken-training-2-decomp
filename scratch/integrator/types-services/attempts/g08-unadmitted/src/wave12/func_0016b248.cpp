#include "game/event_route.h"
#include "game/sound_track.h"
void DrawCommandState::reset()
{
    unsigned char *old_buffer = buffer;
    InputDeviceState::reset();
    func_0016b260(old_buffer);
}
