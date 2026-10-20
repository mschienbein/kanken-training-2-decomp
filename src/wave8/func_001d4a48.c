#include "game/layout.h"


void func_001d4a48(struct AudioVoiceGroup *group, unsigned alignment)
{
    int index;
    for (index = 0; index < group->count; ++index) {
        struct AudioChannel *channel = group->channels[index];
        if (channel != 0) {
            unsigned mode = 0;
            switch (alignment) {
            case 0: break;
            case 1: mode = 1; break;
            case 2: mode = 2; break;
            default: break;
            }
            func_001c59fc(channel, mode);
        }
    }
}
