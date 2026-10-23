#define NW_RELEASE
#include "game/text_flow.h"

static float next_flow_y(const TextFlowState *state, float advance)
{
    return state->y + advance;
}

extern "C" void func_0017a5f0(TextFlowState *state,
                              nw::font::PrintContext<wchar_t> *receiver,
                              nw::ut::Rect *bounds, float multiplier)
{
    float advance = state->advance_y * multiplier;
    float next_y, x;
    if (state->flags & 2)
        advance = state->advance_y * state->multiplier;
    next_y = next_flow_y(state, advance);
    x = state->x;
    if (next_y > state->limit_y) {
        if (bounds) {
            bounds->left = x;
            bounds->top = state->y;
        } else {
            nw::font::WideTextWriter &target = *receiver->writer;
            x -= state->advance_x;
            state->x = x;
            float y = state->resetY();
            state->y = y;
            target.m_CursorPos.x = x;
            target.m_CursorPos.y = y;
        }
        return;
    }
    if (bounds) {
        bounds->left = x;
        bounds->bottom = next_flow_y(state, advance);
    } else {
        nw::font::WideTextWriter *target = receiver->writer;
        state->y = next_y;
        target->m_CursorPos.x = x;
        target->m_CursorPos.y = next_y;
    }
}
