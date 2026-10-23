#ifndef GAME_RESET_REFERENCE_H
#define GAME_RESET_REFERENCE_H
#include <stddef.h>
/* The construction runtime supplies untyped element storage. The callback
 * itself has that exact API; its body recovers the concrete element type. */
struct ResetReference { unsigned value; };
struct InitialRegisterState { unsigned values[6]; };
/* [INFERENCE] The native member-array constructor supplies each 24-byte
 * element's address to the return-this constructor callback. */
struct RuntimeMember {
    unsigned words[6];
#ifdef __cplusplus
    RuntimeMember();
#endif
};
struct RuntimeMemberOwner {
    unsigned prefix[2];
    struct RuntimeMember members[32];
#ifdef __cplusplus
    RuntimeMemberOwner();
#endif
};
#ifdef __cplusplus
extern "C" {
#endif
extern unsigned data_00242dc0;
extern struct InitialRegisterState data_00242dc4;
extern struct ResetReference bss_004b2a44[24];
extern void *func_00156da0(void *storage);
extern void *__aeabi_vec_ctor_nocookie_nodtor(void *array, void *(*constructor)(void *),
                                            size_t element_size, size_t element_count);
/* ARM-to-Thumb veneer of the same generic array-construction runtime. */
extern void *func_001911c4(void *array, void *(*constructor)(void *),
                          size_t element_size, size_t element_count);
#ifdef __cplusplus
}
#endif

#endif
