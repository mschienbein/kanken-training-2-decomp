#include "game/bss_0049ecf0.h"

struct W19aWritableQueue {
    struct CommandQueue base;
    unsigned reserved28;
    int option;
    unsigned char reserved30[0x0d];
    unsigned char enabled;
};

void func_001105c0(int option, int value)
{
    struct W19aWritableQueue *queue = (struct W19aWritableQueue *)bss_0049ecf0.writing;
    if (queue != 0) {
        switch (option) {
        case 0x200:
            queue->option = value;
            break;
        case 0x20f:
            queue->enabled = value != 0;
            break;
        }
    }
}
