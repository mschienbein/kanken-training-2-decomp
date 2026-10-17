#include "game/startup.h"
void func_0013de78(int initialized)
{
    if (bss_0028d944.initialized == 0) {
        bss_0028d944.initialized = initialized;
        if (initialized != 0)
            func_00141944();
    }
}
