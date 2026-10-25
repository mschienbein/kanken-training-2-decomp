#include "game/matrix.h"
#include "game/command_buffer.h"
union Matrix34Command {
    struct Matrix34 matrix;
    unsigned words[12];
};
struct CommandHead {
    unsigned first_coefficient;
    unsigned header;
};
union ModelMatrixPacket {
    struct {
        unsigned first_coefficient;
        union Matrix34Command coefficients;
        unsigned suffix;
    } staging;
    struct {
        struct CommandHead head;
        unsigned remaining[12];
    } submitted;
};
struct ModelMatrixState {
    unsigned char reserved00[0x25];
    unsigned char next_row;
    unsigned char reserved26[0x212];
    unsigned prefix[2];
    union ModelMatrixPacket packet;
};
extern void func_00192720(void *destination, const void *source, unsigned size);

void func_001147c0(struct ModelMatrixState *state, const struct Matrix34 *matrix)
{
    struct CommandHead head;
    func_00195334(&state->packet.staging.coefficients.matrix, matrix);
    state->next_row = 3;
    head.first_coefficient = state->packet.staging.coefficients.words[0];
    head.header = 0x00bf02c1;
    state->packet.submitted.head = head;
    func_00192720(data_00242fc8, state->prefix, 0x40);
    data_00242fc8 += 0x40;
    state->next_row = 0;
}
