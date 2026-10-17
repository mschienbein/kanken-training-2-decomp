#ifndef GAME_PROMPT_H
#define GAME_PROMPT_H

struct DisplayObject;
union PromptGuard;
struct PromptManager;

#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct PromptManager {
    float delay;
    struct DisplayObject **objects;
};

union PromptGuard {
    volatile unsigned int word;
    unsigned char initialized;
};

#ifdef __cplusplus
extern "C" {
#endif
extern union PromptGuard data_0023eff8;
extern struct PromptManager data_0023effc;
extern struct PromptManager *func_001c1970(struct PromptManager *manager);
extern struct PromptManager *func_0016e270(void);
extern void func_001c1700(struct PromptManager *manager);
#ifdef __cplusplus
}
#endif

#endif
