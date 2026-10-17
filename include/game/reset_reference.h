#ifndef GAME_RESET_REFERENCE_H
#define GAME_RESET_REFERENCE_H
#include <stddef.h>
struct ResetReference { unsigned value; };
struct InitialRegisterState { unsigned values[6]; };
#ifdef __cplusplus
extern "C" {
#endif
extern unsigned data_00242dc0;
extern struct InitialRegisterState data_00242dc4;
extern struct ResetReference bss_004b2a44[24];
extern struct ResetReference *func_00156da0(struct ResetReference *reference);
extern void *__aeabi_vec_ctor_nocookie_nodtor(void *array, void *(*constructor)(void *),
                                            size_t element_size, size_t element_count);
#ifdef __cplusplus
}
#endif

#endif
