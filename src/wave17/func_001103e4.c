#include "game/functions.h"
#include "game/bss_0049ecf0.h"
#include "game/render_state.h"



struct CommandQueue;


extern void func_0011cc30(void);
extern void func_001181fc(void);
extern void func_0011d12c(void);
extern void func_00117f2c(void);



void func_001103e4(void)
{
    int i;
    if (!bss_0049ecf0.initialized)
        return;
    func_0011d2d4();
    func_0011cc30();
    func_001181fc();
    func_0011d12c();
    func_00117f2c();
    func_0011c77c();
    bss_0049ecf0.initialized = 0;
    func_00122a68();
    for (i = 0; i < 32; ++i) {
        struct CommandQueue *entry = bss_0049ecf0.queues[i];
        while (entry) {
            struct CommandQueue *next = entry->next;
            if (entry->base && data_002429f4.release)
                data_002429f4.release(0x10000, 0x105, entry->id, entry->base);
            if (data_002429f4.release)
                data_002429f4.release(0x10000, 0x100, 0, entry);
            entry = next;
        }
    }
    for (i = 0; i < 32; ++i) {
        struct CommandResource *entry = bss_0049ecf0.resources[i];
        while (entry) {
            struct CommandResource *next = entry->next;
            if (entry->memory && data_002429f4.release)
                data_002429f4.release(entry->module, 0x104, entry->id, entry->memory);
            if (data_002429f4.release)
                data_002429f4.release(0x10000, 0x100, 0, entry);
            entry = next;
        }
    }
    if (data_002429f4.release)
        data_002429f4.release(0x10000, 0x100, 0, data_002429f4.memory);
    data_002429f4.memory = 0;
    data_002429f4.allocate = 0;
    data_002429f4.release = 0;
}
