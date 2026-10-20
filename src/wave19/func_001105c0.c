#include "game/bss_0049ecf0.h"


void func_001105c0(int option, int value)
{
    struct CommandQueue *queue = bss_0049ecf0.writing;
    if (queue != 0) {
        switch (option) {
        case 0x200:
            queue->option = value;
            break;
        case 0x20f:
            queue->flush = value != 0;
            break;
        }
    }
}
