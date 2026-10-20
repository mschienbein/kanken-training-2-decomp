#ifndef GAME_TEXT_CHOICES_H
#define GAME_TEXT_CHOICES_H

#include <stddef.h>
#include "game/runtime.h"

/* [INFERENCE] Counted, owned UTF-16 choices; the constructor and both indexed
 * consumers share the skipped count word and the twelve entry slots. */
struct TextChoices {
    int count;
    unsigned short *entries[12];
#ifdef __cplusplus
    TextChoices(int count, ...) throw();
    static __forceinline void *operator new(size_t size, const void *tag) throw()
    {
        return func_0019ffa4(size, tag);
    }
#endif
};
#ifdef __cplusplus
extern "C" {
#endif
extern struct TextChoices *bss_00290ec4[12];
void func_00203a68(void);
#ifdef __cplusplus
}
#endif

#endif
