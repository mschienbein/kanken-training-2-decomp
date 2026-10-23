#include "game/w11_control.h"
#include "game/task_runtime.h"

static void fade_handle(struct PlaybackHandle *handle, int frames)
{
    struct AnimatedObject *controller = handle->controller;
    if (controller)
        func_00183f24(controller, frames);
}

void func_0017433c(int channel, int duration)
{
    int end, frames;
    struct RuntimePlaybackChannel *entry;
    float rate;
    if (!bss_0029f540.active)
        return;
    rate = func_0017adb4();
    if (channel == -1) {
        channel = 0;
        end = 4;
    } else {
        end = channel + 1;
    }
    frames = (int)((float)duration / rate);
    entry = &bss_0029f540.channels[channel];
    for (; channel < end; ++channel, ++entry) {
        if (entry->resource != -1) {
            entry->resource = -1;
            fade_handle(entry->handle, frames);
        }
    }
}
