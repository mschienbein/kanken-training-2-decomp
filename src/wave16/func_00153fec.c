#include "game/audio_voice.h"
#include "game/functions.h"

struct AudioChannel;


struct HandleManager;
extern struct HandleManager bss_004b6888;
extern struct AudioChannel *func_00154838(struct HandleManager *pool,
    int priority, void (*callback)(struct AudioChannel *, struct AudioVoiceGroup *),
    struct AudioVoiceGroup *group);

struct AudioChannel *func_00153fec(int priority,
    void (*callback)(struct AudioChannel *, struct AudioVoiceGroup *),
    struct AudioVoiceGroup *group)
{
    return func_00154838(&bss_004b6888, priority, callback, group);
}
