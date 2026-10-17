#include "game/shader.h"
#include "game/matrix.h"

struct MatrixIdentityW8C : Matrix34 {
    MatrixIdentityW8C()
    {
        elements[0] = 1.0f; elements[1] = 0.0f; elements[2] = 0.0f; elements[3] = 0.0f;
        elements[4] = 0.0f; elements[5] = 1.0f; elements[6] = 0.0f; elements[7] = 0.0f;
        elements[8] = 0.0f; elements[9] = 0.0f; elements[10] = 1.0f; elements[11] = 0.0f;
    }
};
__forceinline const Matrix34 *matrix34Identity()
{
    /* [INFERENCE] callee identity: the guard-release call was inlined by armlink. */
    static MatrixIdentityW8C identity_value;
    return &identity_value;
}
struct ShaderGeometryBuffers {
    unsigned char reserved00[0x194];
    unsigned buffers[2];
};
struct RendererState {
    unsigned char reserved00[0x15c];
    unsigned program;
    unsigned reserved160;
    Matrix34 model_matrix;
    unsigned buffers[2];
    int uniforms[79];
    unsigned char reserved2d8[8];
    unsigned char initialized;
};

/* INTEGRATOR: move to include/game/renderer_state.h */
extern "C" {
extern const char *data_00242e48[79];
void func_00116c1c(struct ShaderGeometryBuffers *state);

}

extern "C" void func_0010dd88(RendererState *state)
{
    func_00195334(&state->model_matrix, matrix34Identity());
    func_00116c1c(reinterpret_cast<ShaderGeometryBuffers *>(state));
    for (int i = 0; i < 79; ++i) {
        if (data_00242e48[i] == 0)
            state->uniforms[i] = 0;
        else
            state->uniforms[i] = func_00191b08(state->program, data_00242e48[i]);
    }
    state->initialized = 1;
}
