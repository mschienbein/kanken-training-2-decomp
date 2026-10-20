#include "game/render_parameter.h"

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
struct W22ExtendedInput : TouchHistory {
    TouchBounds bounds;
    unsigned reserved34[4];
    W22ExtendedInput() : TouchHistory(), bounds(0, 0, 0, 0) {}
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
struct W22WeightedHandle {
    void *handle;
    float weight;
    W22WeightedHandle() : handle(0), weight(0) {}
};
struct W22WeightedGroup : W22WidgetMask {
    W22WeightedHandle *handles[10];
    unsigned char active, reserved35[3];
    W22ExtendedInput *inputs[3];
    W22WeightedGroup();
};

W22WeightedGroup::W22WeightedGroup() : W22WidgetMask(32)
{
    for (int i = 0; i < 10; ++i)
        if (!handles[i])
            handles[i] = new (data_002232d5) W22WeightedHandle;
    inputs[0] = new (data_002232d5) W22ExtendedInput;
    inputs[1] = new (data_002232d5) W22ExtendedInput;
    inputs[2] = new (data_002232d5) W22ExtendedInput;
    active = 1;
}
