#include "game/bss_0049ecf0.h"




void func_00110548(void)
{
    struct CommandQueue *queue = bss_0049ecf0.writing;
    if (!queue || bss_0049ecf0.pending)
        return;
    bss_0049ecf0.executing = queue;
    bss_0049ecf0.enabled = 1;
    if (queue->cursor < queue->count) {
        bss_0049ecf0.pending = 1;
        if ((unsigned)queue->option == 0x300) {
            func_0012fb38();
            func_00190878();
            func_0012fb48();
        }
    }
}
