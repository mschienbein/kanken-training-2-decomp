#include "game/screen.h"
#include "game/display_depth.h"
#include "game/functions.h"

/* [INFERENCE] Descriptive value-snapshot type: each instance preserves eight
 * float depths while the per-display registration calls execute. No historical
 * class or out-of-line constructor identity is claimed. */
struct DepthSnapshot {
    float values[8];
    DepthSnapshot(float value0, float value1, float value2, float value3, float value4, float value5, float value6, float value7)
    {
        values[0] = value0;
        values[1] = value1;
        values[2] = value2;
        values[3] = value3;
        values[4] = value4;
        values[5] = value5;
        values[6] = value6;
        values[7] = value7;
    }
};

void func_00175848(void)
{
    func_0015e2a4(1);
    int index = 0;
    DepthSnapshot upper(data_00218000.upper.values[0], data_00218000.upper.values[1], data_00218000.upper.values[2], data_00218000.upper.values[3], data_00218000.upper.values[4], data_00218000.upper.values[5], data_00218000.upper.values[6], data_00218000.upper.values[7]);
    DepthSnapshot lower(data_00218000.lower.values[0], data_00218000.lower.values[1], data_00218000.lower.values[2], data_00218000.lower.values[3], data_00218000.lower.values[4], data_00218000.lower.values[5], data_00218000.lower.values[6], data_00218000.lower.values[7]);
    const float *upper_values = upper.values;
    const float *lower_values = lower.values;
    for (; index < 8; ++index) {
        func_00196144(3, index);
        func_00196144(4, index);
        func_00181420(3, index, upper_values[index]);
        func_00181420(4, index, lower_values[index]);
    }
    func_0019eb24();
}
