#define NW_RELEASE
#include <cstring>
#include "game/text_flow.h"

static void set_cursor(nw::font::CharWriter *writer, float x, float y)
{
    writer->SetCursor(x, y);
}

extern "C" void func_001af59c(TextFlowState *state, nw::font::PrintContext<wchar_t> *receiver,
                   nw::ut::Rect *bounds)
{
    float advance = state->advance_x;
    float y;
    if (state->flags & 2) {
        float multiplier = state->multiplier;
        advance = multiplier * advance;
    }
    y = state->y;
    if (y + advance > state->limit_y) {
        if (bounds) {
            std::memcpy(&bounds->left, &state->x, sizeof(float));
            std::memcpy(&bounds->top, &state->y, sizeof(float));
        } else {
            nw::font::WideTextWriter *target = receiver->writer;
            std::memcpy(&state->x, &state->origin_x, sizeof(float));
            float line_advance = state->advance_y;
            y = y + line_advance;
            state->y = y;
            std::memcpy(&target->m_CursorPos.x, &state->x, sizeof(float));
            target->m_CursorPos.y = y;
        }
        return;
    }
    if (bounds) {
        bounds->left = state->x + advance;
        std::memcpy(&bounds->bottom, &state->y, sizeof(float));
    } else {
        nw::font::WideTextWriter *target = receiver->writer;
        state->x += advance;
        set_cursor(target, state->x, state->y);
    }
}

