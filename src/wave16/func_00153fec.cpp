#include "game/audio_voice.h"
#include "game/functions.h"
#include "game/service_runtime.h"

struct AudioChannel;


extern "C" struct AudioChannel *func_00153fec(int priority,
    void (*callback)(struct AudioChannel *, nw::snd::internal::driver::Voice *),
    nw::snd::internal::driver::Voice *group)
{
    return func_00154838(&bss_004b6888, priority, callback, group);
}
