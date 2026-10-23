#include "game/input_device.h"

static bool update_dirty(W19AlignmentState *state, bool changed)
{
    state->dirty |= changed ? 1 : 0;
    return changed;
}

static unsigned position_component(unsigned char position, unsigned axis)
{
    return axis == 0 ? static_cast<unsigned char>(position % 3) : position / 3;
}

static unsigned replace_position(const unsigned char &position, unsigned value, unsigned axis)
{
    return axis == 0 ? (position / 3) * 3 + value : position % 3 + value * 3;
}

void func_0015e830(AlignmentHandle *alignment, unsigned value)
{
    W19AlignmentState *state = alignment->state;
    if (update_dirty(state, position_component(state->alignment, 1) != value))
        state->alignment = replace_position(state->alignment, value, 1);
    state = alignment->state;
    unsigned char text = state->text_alignment;
    unsigned char low = text & 0x0f;
    unsigned high = text & 0xf0;
    state->text_alignment = high | replace_position(low, value, 1);
}
