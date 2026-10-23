#ifndef GAME_TEXT_FLOW_H
#define GAME_TEXT_FLOW_H
#include <nn/Assert.h>
#include <GLES2/gl2extern.h>
#include <nw/font/font_WideTextWriter.h>
#include <nw/ut/ut_Rect.h>
/* [INFERENCE] Shared tag-flow state. The receiver is the actual native print
 * context and writer, not a second state record with coincident cursor offsets. */
struct TextFlowState {
    const void *vtable;
    unsigned char flags;
    unsigned char reserved05[3];
    int remaining;
    unsigned reserved0c;
    float fixed_width, height;
    unsigned reserved18[2];
    float multiplier;
    float origin_x, origin_y;
    float x, y;
    unsigned char reserved34[8];
    float advance_x, advance_y, limit_y;
    float resetY() const { return origin_y; }
};
extern "C" {
void func_0017a5f0(TextFlowState *state,
                  nw::font::PrintContext<wchar_t> *receiver,
                  nw::ut::Rect *bounds, float multiplier);
void func_001af59c(TextFlowState *state,
                  nw::font::PrintContext<wchar_t> *receiver,
                  nw::ut::Rect *bounds);
}
#endif
