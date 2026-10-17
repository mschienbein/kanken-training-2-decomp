#ifndef GAME_SHADER_H
#define GAME_SHADER_H
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
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_00116f40(struct ShaderConfiguration *state, const void *binary, unsigned size);
extern struct ShaderRegistry bss_002d4250;
extern void func_0019b3b0(unsigned program, unsigned shader);
extern void func_0019b344(unsigned program);
extern void *func_00191968(unsigned size, unsigned char alignment);
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
extern void func_0019194c(int location, unsigned value);
extern void func_00191aec(void *allocation);
#ifdef __cplusplus
}
#endif
#endif
