#ifndef GAME_TEXT_CHOICES_H
#define GAME_TEXT_CHOICES_H

#include <stddef.h>
#include "game/runtime.h"

#ifdef __cplusplus
/* [INFERENCE] Counted, owned UTF-16 choices; the constructor copies each input. */
struct TextChoices {
    int count;
    unsigned short *entries[12];

    TextChoices(int count, ...) throw();
    static __forceinline void *operator new(size_t size, const void *tag) throw()
    {
        return func_0019ffa4(size, tag);
    }
};
extern "C" {
extern TextChoices *bss_00290ec4[12];
void func_00203a68(void);
}
#endif

#endif
