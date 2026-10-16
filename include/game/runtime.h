#ifndef GAME_RUNTIME_H
#define GAME_RUNTIME_H

/* Process-wide helpers used across subsystems. */

#ifdef __cplusplus
extern "C" {
#endif

extern void func_001852f4(void *allocation);   /* releases a heap allocation */
extern void func_0019f568(void);               /* fatal-error / assertion handler */
extern void func_001ef888(void *allocation);   /* releases an array allocation */

#ifdef __cplusplus
}
#endif

#endif
