#include "game/aux_effect.h"

extern const unsigned int data_00242fa4[];

void func_001472a4(struct SoundSubsystem *manager)
{
    if (!manager->active) {
        int channel;
        int index;
        unsigned char mode;
        for (channel = 0; channel < 2; channel++) {
            func_00148358(channel, &manager->saved_callbacks[channel], &manager->saved_contexts[channel]);
            func_00183614(channel, 0, 0);
        }
        for (index = 0; index < 128; index++)
            manager->tables[index] = 0;
        {
            const unsigned int *table = data_00242fa4;
            const void **slot = &manager->tables[1];
            *slot++ = table++;
            *slot++ = table++;
            *slot++ = table++;
            *slot++ = table++;
            *slot = table;
        }
        func_00148370(1.0f);
        mode = func_001487f4();
        switch (mode) {
        case 0: manager->output_mode = 0; break;
        case 1: manager->output_mode = 1; break;
        case 2: manager->output_mode = 2; break;
        }
        manager->active = 1;
    }
}
