#define NW_RELEASE
#include "nn/Assert.h"
#include "GLES2/gl2extern.h"
#include "game/text_writer.h"
#include <cstring>

struct MarkupStateR41258 {
    const void *vtable;
    unsigned flags;
    int remaining;
    unsigned reserved0c;
    float fixedWidth, height;
    unsigned reserved18[2];
    float width;
    nn::math::VEC2 origin, previous;
    unsigned reserved34[2];
    float savedFixedWidth, lineHeight;
};


extern "C" unsigned char func_001af644(MarkupStateR41258 *state,
    unsigned short code, nw::font::PrintContext<wchar_t> *context)
{
    unsigned char active = state->flags & 1u;
    if (!active)
        return active;
    float digit = code - 1;
    switch (state->remaining) {
    case 4:
        state->fixedWidth += digit * 10.0f;
        break;
    case 3:
        state->fixedWidth += digit;
        break;
    case 2:
        state->width += digit * 10.0f;
        break;
    case 1:
        state->width += digit;
        break;
    }
    if (--state->remaining <= 0) {
        nw::font::WideTextWriter *writer = context->writer;
        state->width /= state->fixedWidth;
        writer->m_FixedWidth = state->fixedWidth;
        state->height = func_0017a6c8(writer);
        std::memcpy(&state->savedFixedWidth, &writer->m_FixedWidth, sizeof(float));
        state->lineHeight = func_0017a690(writer);
        /* [INFERENCE] Save each coordinate representation before restoring the
         * cursor's contiguous two-coordinate value. */
        struct { float x, y; } restored;
        std::memcpy(&restored.x, &state->origin.x, sizeof(float));
        std::memcpy(&state->previous.x, &restored.x, sizeof(float));
        std::memcpy(&restored.y, &state->origin.y, sizeof(float));
        std::memcpy(&state->previous.y, &restored.y, sizeof(float));
        std::memcpy(&writer->m_CursorPos.x, &restored, sizeof(restored));
        state->flags &= ~1u;
    }
    return true;
}
