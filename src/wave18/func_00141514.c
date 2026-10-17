#include "game/bss_0049ecf0.h"

struct ExtendedCommandQueue {
    struct CommandQueue queue;
    int cursor;
    unsigned capacity;
    unsigned captured_size;
    struct CommandPatch *patch;
    unsigned reserved38;
    unsigned char complete;
    unsigned char flush;
};
extern void func_00195f7c(void);

void func_00141514(void)
{
    if (bss_0049ecf0.writing) {
        struct ExtendedCommandQueue *queue = (struct ExtendedCommandQueue *)bss_0049ecf0.writing;
        unsigned char previous = queue->flush;
        queue->flush = 1;
        func_00195f7c();
        if (!previous)
            ((struct ExtendedCommandQueue *)bss_0049ecf0.writing)->flush = 0;
    }
}
