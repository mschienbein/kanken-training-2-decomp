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
struct W22LargeHandle {
    void *handle;
    unsigned char reserved04[0x30];
    W22LargeHandle() : handle(0) {}
};
struct W22LargeResourceGroup : W22WidgetMask {
    W22LargeHandle *handles[27];
    unsigned reserved78[3];
    TouchFrame *inputs[2];
    W22LargeResourceGroup();
    ~W22LargeResourceGroup();
};

W22LargeResourceGroup::W22LargeResourceGroup() : W22WidgetMask(32)
{
    int i;
    for (i = 0; i < 27; ++i)
        if (!handles[i])
            handles[i] = new (data_002232d5) W22LargeHandle;
    for (i = 0; i < 2; ++i)
        inputs[i] = new (data_002232d5) TouchFrame;
}
