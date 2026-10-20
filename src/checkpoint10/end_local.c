#include <stdint.h>

extern unsigned bss_00243908[];
extern unsigned bss_004e32ec[];

void func_00100024(void)
{
    unsigned *word = bss_00243908;
    unsigned *end = bss_004e32ec;
    for (;;) { if ((uintptr_t)word >= (uintptr_t)end) return; *word++ = 0; }
}
