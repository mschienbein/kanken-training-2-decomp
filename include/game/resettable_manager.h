#ifndef GAME_RESETTABLE_MANAGER_H
#define GAME_RESETTABLE_MANAGER_H
#include "game/exp_heap.h"



#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct ResettableManager;

struct ResettableManager : AllocatorBase {
    unsigned char reserved18[0x0c];
    void *current;
};

extern "C" {
extern const unsigned data_00223bf0[];

extern void func_00134170(void *storage);
}
#endif

#endif
