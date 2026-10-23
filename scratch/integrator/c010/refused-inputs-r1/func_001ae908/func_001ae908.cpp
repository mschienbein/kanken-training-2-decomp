#include "game/render_parameter.h"
#include "game/animation.h"

/* [INFERENCE] Descriptive class/member names; layouts and native constructor contracts follow the call sites. */
struct TouchHistory {
    unsigned char active, released, reserved02[2];
    float position[2];
    unsigned char previous_active, previous_released, reserved0e[2];
    float previous_position[2];
    unsigned char tracking, reserved19[3];
    float start_position[2];
    TouchHistory();
};
struct TouchBounds {
    float x, y, width, height;
    TouchBounds(float x, float y, float width, float height);
};
struct TouchFrame : TouchHistory {
    TouchBounds bounds;
    unsigned state;
    TouchFrame() : TouchHistory(), bounds(0, 0, 0, 0), state(0)
    {
        func_001670e4(&bounds.x, 0, 0, 0, 0);
    }
};
struct W22WidgetMask {
    int bit_count, word_count;
    unsigned mask[1];
    W22WidgetMask(int count)
    {
        bit_count = count;
        word_count = (count + 31) / 32;
        for (int i = 0; i < word_count; ++i)
            mask[i] = 0;
    }
};
struct W22Handle { void *handle; W22Handle() : handle(0) {} };
struct W22ResourceGroup : W22WidgetMask {
    W22Handle *handles[25];
    TouchFrame *inputs[12];
    W22ResourceGroup();
    ~W22ResourceGroup();
};

W22ResourceGroup::W22ResourceGroup() : W22WidgetMask(32)
{
    int i;
    for (i = 0; i < 25; ++i)
        if (!handles[i])
            handles[i] = new (data_002232d5) W22Handle;
    for (i = 0; i < 12; ++i)
        inputs[i] = new (data_002232d5) TouchFrame;
}
