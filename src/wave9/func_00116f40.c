struct ShaderConfiguration {
    unsigned program, texture;
    unsigned char reserved08[0x50];
    int uniforms[35];
    unsigned char reservede4[8];
    unsigned char initialized;
};

extern unsigned func_0019b13c(void);
extern unsigned func_0019b01c(unsigned kind);
extern void func_00199e10(int count, const unsigned *shaders, unsigned format,
                          const void *binary, unsigned size);
extern void func_00199d54(unsigned program_id, unsigned shader_id);
extern void func_0019b254(unsigned shader_id);
extern void func_00199c08(unsigned program_id, unsigned location, const char *name);
extern void func_00198170(unsigned program_id);
extern void func_0018d3d8(int count, unsigned *ids);
extern int func_00191b08(unsigned program_id, const char *name);

void func_00116f40(struct ShaderConfiguration *state, const void *binary, unsigned size)
{
    unsigned shader;
    state->program = func_0019b13c();
    shader = func_0019b01c(0x8b31);
    func_00199e10(1, &shader, 0x6000, binary, size);
    func_00199d54(state->program, shader);
    func_0019b254(shader);
    func_00199d54(state->program, ~0u);
    func_00199c08(state->program, 0, "aPosition");
    func_00199c08(state->program, 1, "aPositionZ");
    func_00199c08(state->program, 2, "aColor");
    func_00199c08(state->program, 3, "aTexCoord");
    func_00198170(state->program);
    func_0018d3d8(1, &state->texture);
    state->uniforms[0] = func_00191b08(state->program, "uProjection");
    state->uniforms[1] = func_00191b08(state->program, "uModelView");
    state->uniforms[2] = func_00191b08(state->program, "dmp_FragmentLighting.enabled");
    state->uniforms[3] = func_00191b08(state->program, "dmp_Texture[0].samplerType");
    state->uniforms[4] = func_00191b08(state->program, "dmp_Fog.mode");
    state->uniforms[5] = func_00191b08(state->program, "dmp_FragOperation.enableAlphaTest");
    state->uniforms[6] = func_00191b08(state->program, "dmp_FragOperation.alphaRefValue");
    state->uniforms[7] = func_00191b08(state->program, "dmp_FragOperation.alphaTestFunc");
    state->uniforms[8] = func_00191b08(state->program, "dmp_TexEnv[3].srcRgb");
    state->uniforms[9] = func_00191b08(state->program, "dmp_TexEnv[3].srcAlpha");
    state->uniforms[10] = func_00191b08(state->program, "dmp_TexEnv[3].operandRgb");
    state->uniforms[11] = func_00191b08(state->program, "dmp_TexEnv[3].operandAlpha");
    state->uniforms[12] = func_00191b08(state->program, "dmp_TexEnv[3].combineRgb");
    state->uniforms[13] = func_00191b08(state->program, "dmp_TexEnv[3].combineAlpha");
    state->uniforms[14] = func_00191b08(state->program, "dmp_TexEnv[3].scaleRgb");
    state->uniforms[15] = func_00191b08(state->program, "dmp_TexEnv[3].scaleAlpha");
    state->uniforms[16] = func_00191b08(state->program, "dmp_TexEnv[3].constRgba");
    state->uniforms[17] = func_00191b08(state->program, "dmp_TexEnv[4].srcRgb");
    state->uniforms[18] = func_00191b08(state->program, "dmp_TexEnv[4].srcAlpha");
    state->uniforms[19] = func_00191b08(state->program, "dmp_TexEnv[4].operandRgb");
    state->uniforms[20] = func_00191b08(state->program, "dmp_TexEnv[4].operandAlpha");
    state->uniforms[21] = func_00191b08(state->program, "dmp_TexEnv[4].combineRgb");
    state->uniforms[22] = func_00191b08(state->program, "dmp_TexEnv[4].combineAlpha");
    state->uniforms[23] = func_00191b08(state->program, "dmp_TexEnv[4].scaleRgb");
    state->uniforms[24] = func_00191b08(state->program, "dmp_TexEnv[4].scaleAlpha");
    state->uniforms[25] = func_00191b08(state->program, "dmp_TexEnv[4].constRgba");
    state->uniforms[26] = func_00191b08(state->program, "dmp_TexEnv[5].srcRgb");
    state->uniforms[27] = func_00191b08(state->program, "dmp_TexEnv[5].srcAlpha");
    state->uniforms[28] = func_00191b08(state->program, "dmp_TexEnv[5].operandRgb");
    state->uniforms[29] = func_00191b08(state->program, "dmp_TexEnv[5].operandAlpha");
    state->uniforms[30] = func_00191b08(state->program, "dmp_TexEnv[5].combineRgb");
    state->uniforms[31] = func_00191b08(state->program, "dmp_TexEnv[5].combineAlpha");
    state->uniforms[32] = func_00191b08(state->program, "dmp_TexEnv[5].scaleRgb");
    state->uniforms[33] = func_00191b08(state->program, "dmp_TexEnv[5].scaleAlpha");
    state->uniforms[34] = func_00191b08(state->program, "dmp_TexEnv[5].constRgba");
    state->initialized = 1;
}
