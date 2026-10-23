#include "game/shader.h"
#include "game/matrix.h"


extern "C" void func_0010dd88(RendererState *state)
{
    func_00195334(&state->model_matrix, matrix34Identity());
    func_00116c1c(state);
    for (int i = 0; i < 79; ++i) {
        if (data_00242e48[i] == 0)
            state->uniforms[i] = 0;
        else
            state->uniforms[i] = func_00191b08(state->program, data_00242e48[i]);
    }
    state->initialized = 1;
}
