#include "game/bss_0049ecf0.h"

extern void func_00195f7c(void);

void func_00141514(void)
{
    if (bss_0049ecf0.writing) {
        struct CommandQueue *queue = bss_0049ecf0.writing;
        unsigned char previous = queue->flush;
        queue->flush = 1;
        func_00195f7c();
        if (!previous)
            bss_0049ecf0.writing->flush = 0;
    }
}
