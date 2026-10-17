#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "game/pool_list.h"

/* Cross-unit interfaces without a more specific subsystem header. */
struct ObjectManager;

/* Domain entries use the same count-and-sentinel intrusive list as pool users. */


struct RegistryEntry {
    const void *type;
    struct PoolLink link;
};

/* Shared by the timing reset and the object that embeds its state. */
struct TimingState {
    float elapsed;
    float duration;
    unsigned int count;
    unsigned char active;
    unsigned char paused;
    unsigned char complete;
};

/* Generic destructor registration erases the concrete object-pointer type.
 * Typed destructor callbacks are converted explicitly at the registration site;
 * all object pointers use the same single-word AAPCS parameter ABI. */
typedef void (*ObjectDestructor)(void *object);

#ifdef __cplusplus
extern "C" {
#endif

extern void func_00100000(void);
extern int func_00191c80(volatile unsigned int *guard);
extern int func_00122500(void);
extern void func_00122510(void);
extern void *func_00186488(void *object);
extern void func_00186fbc(unsigned *state);
extern void *func_00125b54(void *object);
extern unsigned func_001424c0(void);
extern unsigned func_001424d0(void);
extern int func_00144224(int value);
extern void func_00147260(struct ObjectManager *object);
extern struct PoolLink *func_0018d868(struct PoolList *registry, struct PoolLink *node);
extern void func_0017e7bc(struct PoolList *registry, struct RegistryEntry *entry);
extern struct PoolLink *func_0017e790(struct PoolList *registry, struct RegistryEntry *entry);
extern struct ObjectManager *func_00185d1c(void);
extern struct PoolList *func_0017e7c4(void);
extern void func_0017e818(struct TimingState *state);
extern unsigned func_0019d048(void);
extern unsigned func_0019d058(void);
extern void *func_001c5b88(void *object);
extern void func_001c6adc(void);
extern void func_001db134(void *object);
extern void func_00215ec4(volatile unsigned int *guard);
extern void func_00216140(void *object, ObjectDestructor destroy, void (*image_base)(void));

#ifdef __cplusplus
}
#endif

#endif
