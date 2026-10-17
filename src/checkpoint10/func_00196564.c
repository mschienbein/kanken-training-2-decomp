struct ShaderProgramSet {
    unsigned programs[6];
    int current;
};
extern struct ShaderProgramSet bss_002d4250;
extern int func_00191b08(unsigned program_id, const char *name);
extern void func_0011da50(int location, unsigned count, int transpose, const float *values);

void func_00196564(const float *values)
{
    int location = func_00191b08(bss_002d4250.programs[bss_002d4250.current], "uModelView");
    func_0011da50(location, 1, 1, values);
}
