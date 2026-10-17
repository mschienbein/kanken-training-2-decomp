#include "game/shader.h"
struct ShaderProgramSet {
    unsigned programs[6];
    int current;
};


extern void func_0011da50(int location, unsigned count, int transpose, const float *values);

void func_001965a4(const float *values)
{
    int location = func_00191b08(bss_002d4250.programs[bss_002d4250.current], "uProjection");
    func_0011da50(location, 1, 1, values);
}
