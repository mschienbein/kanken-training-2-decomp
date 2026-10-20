#include "game/functions.h"
#include "game/audio_dsp.h"
#include "game/service_runtime.h"

/* The indexed table has no recovered bound; the selector is a second typed view. */





void func_00180e14(struct ServiceCoordinator *driver, float volume)
{
    func_00182210();
    if (func_0018f018()) {
        struct VoiceDspParameters *command = driver->voice[driver->completion_slot];
        command->parameters.master_volume = volume;
        command->flags |= 0x10000;
        func_00182204();
        return;
    }
    func_00182204();
}
