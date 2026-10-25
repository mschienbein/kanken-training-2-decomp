#include "game/matrix.h"
#include "game/command_buffer.h"
union Matrix44Command {
    struct Matrix44 matrix;
    unsigned words[16];
};
struct ProjectionState {
    unsigned char reserved00[0x28];
    unsigned prefix[2];
    unsigned first_coefficient;
    union Matrix44Command coefficients;
    unsigned suffix[5];
};
extern void func_00192720(void *destination, const void *source, unsigned size);

void func_00114778(struct ProjectionState *state, const struct Matrix44 *matrix)
{
    func_001944f0(&state->coefficients.matrix, matrix);
    state->first_coefficient = state->coefficients.words[0];
    state->coefficients.words[0] = 0x013f02c1;
    func_00192720(data_00242fc8, state->prefix, 0x60);
    data_00242fc8 += 0x60;
}
