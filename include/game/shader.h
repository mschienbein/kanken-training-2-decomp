#ifndef GAME_SHADER_H
#define GAME_SHADER_H
#include "game/matrix.h"
/* [INFERENCE] Real renderer receiver forwarded unchanged to buffer setup,
 * distinct from ShaderConfiguration despite compatible individual fields. */
struct RendererState {
    unsigned char reserved00[0x15c];
    unsigned program;
    unsigned reserved160;
    struct Matrix34 model_matrix;
    unsigned buffers[2];
    int uniforms[79];
    unsigned char reserved2d8[8];
    unsigned char initialized;
};
struct ShaderConfiguration {
    unsigned program, texture;
    unsigned char reserved08[0x50];
    int uniforms[35];
    unsigned char reservede4[8];
    unsigned char initialized;
    unsigned char reserveded[0x43];
};
struct ShaderRegistry {
    unsigned programs[3];
    unsigned shaders[3];
    int current;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_00116f40(struct ShaderConfiguration *state, const void *binary, unsigned size);
extern struct ShaderConfiguration *func_00117684(struct ShaderConfiguration *state);
extern void func_00193af4(unsigned index);
extern struct ShaderRegistry bss_002d4250;
extern void func_0019b3b0(unsigned program, unsigned shader);
extern void func_0019b344(unsigned program);
extern void *func_00191968(unsigned size, unsigned char alignment);
extern void func_0018f0dc(int count, const unsigned *handles);
extern unsigned func_0019b13c(void);
extern unsigned func_0019b01c(unsigned kind);
extern void func_00199e10(int count, const unsigned *shaders, unsigned format,
                          const void *binary, unsigned size);
extern void func_00199d54(unsigned program, unsigned shader);
extern void func_0019b254(unsigned shader);
extern void func_00199c08(unsigned program, unsigned location, const char *name);
extern void func_00198170(unsigned program);
extern void func_00197c68(unsigned program);
extern int func_00191b08(unsigned program, const char *name);
extern void func_0011da50(int location, unsigned count, int transpose, const void *values);
extern void func_00127ddc(int location, const unsigned *values, unsigned type, int count);
extern void func_0019194c(int location, unsigned value);
extern void func_00191aec(void *allocation);
extern const char *data_00242e48[79];
extern void func_00116c1c(struct RendererState *state);
extern void func_0010dd88(struct RendererState *state);
#ifdef __cplusplus
}
#endif
#endif
