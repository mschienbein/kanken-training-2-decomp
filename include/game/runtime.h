#ifndef GAME_RUNTIME_H
#define GAME_RUNTIME_H

/* Process-wide helpers used across subsystems. */

#ifdef __cplusplus
extern "C" {
#endif

extern void func_0011ef54(long long nanoseconds); /* sleep wrapper */
extern void func_0011ef58(long long nanoseconds); /* underlying sleep operation */
extern void *func_0013714c(void *object);      /* empty destructor wrapper */
extern void *func_00137150(void *object);      /* preserves the allocation pointer */
extern void func_001852f4(void *allocation);   /* releases a heap allocation */
extern void *func_0018f7f0(void *object);      /* preserves the allocation pointer */
extern void func_0019f568(void);               /* fatal-error / assertion handler */
extern void *func_001ef880(void *object);      /* array-element destroy callback */
extern void *func_001ef884(void *object);      /* preserves the element pointer */
extern void func_001ef888(void *allocation);   /* releases an array allocation */

#ifdef __cplusplus
}
#endif

#endif
