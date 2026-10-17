#include "game/bss_0049ecf0.h"
struct CommandQueue;

void func_00110600(unsigned event, void (*callback)(int event))
{
    switch (event) {
    case 0x400:
        bss_0049ecf0.begin_callback = callback;
        break;
    case 0x401:
        bss_0049ecf0.callback = callback;
        break;
    case 0x402:
        bss_0049ecf0.begin_callback = callback;
        bss_0049ecf0.callback = callback;
        break;
    }
}
