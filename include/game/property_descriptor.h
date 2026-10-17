#ifndef GAME_PROPERTY_DESCRIPTOR_H
#define GAME_PROPERTY_DESCRIPTOR_H

#include <stdarg.h>
#include "game/screen.h"
#include "game/runtime.h"

struct FloatPropertyDescriptor {
    struct NameTable base;
    float x;
    float y;
    float z;
};

#ifdef __cplusplus
extern "C" {
#endif

extern const struct NameTableVTable data_00223664;
extern const struct NameTableVTable data_00223678;
extern struct NameTable *bss_002466f8[6];
extern struct NameTable *bss_00290d28[5];
extern struct NameTable *bss_00290e60[4];
extern struct NameTable *bss_00290f60[6];
extern struct NameTable *bss_00290f8c[6];
extern struct NameTable *bss_00290fc4[4];
extern struct NameTable *data_0023ecc4[2];
extern struct NameTable *data_0023efdc[2];
extern struct NameTable *data_0023f004[2];
extern struct NameTable *data_0023f014[2];

extern struct NameTable *func_0016e06c(struct NameTable *object);
extern void func_0015f5a4(struct NameTable *object, int category, int identifier,
                          const char *name);
extern struct NameTable *func_00164d5c(struct NameTable *object, int category,
                                      int identifier, const char *name,
                                      int count, ...);
extern void func_001b7d50(struct NameTable *object, int count, va_list arguments);
extern struct FloatPropertyDescriptor *func_00164ddc(
    struct FloatPropertyDescriptor *object, int category, int identifier,
    const char *name, float x, float y, float z, int count, ...);
extern void func_00207274(void);
extern void func_0020e3b0(void);
extern void func_002122f4(void);
extern void func_002126f0(void);
extern void func_00212874(void);
extern void func_002129e4(void);
extern void func_00213140(void);
extern void func_00214468(void);
extern void func_002145f4(void);
extern void func_0021521c(void);

#ifdef __cplusplus
}
#endif

#endif
